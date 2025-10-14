/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiPollable.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/06 13:14:33 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 10:55:09 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/CgiPollable.hpp"

#include "serve/Connection.hpp"
#include "serve/EventLoop.hpp"

static inline void setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags >= 0)
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// CONSTRUCTORS
// ____________________________________________________________________________
CgiPollable::CgiPollable(EventLoop *loop, Connection *ownerConnection, const HttpRequest &request,
						 const Location &location, const CgiConfig &cgiConfig)
	: loop(loop), ownerConnection(ownerConnection), request(request), location(location), cgiConfig(cgiConfig), pid(-1),
	  stdin_fd(-1), stdout_fd(-1)
{
}

CgiPollable::~CgiPollable()
{
	unregisterPollables();

	int st = 0;
	extractAndWaitChild(st);
}

void CgiPollable::removeStdoutFromLoop()
{
	if (loop && stdout_reg_fd >= 0)
	{
		loop->remove(stdout_reg_fd);
		stdout_reg_fd = -1;
	}
}
void CgiPollable::removeStdinFromLoop()
{
	if (loop && stdin_reg_fd >= 0)
	{
		loop->remove(stdin_reg_fd);
		stdin_reg_fd = -1;
	}
}

// START
// ____________________________________________________________________________
bool CgiPollable::start()
{
	// 1) Script path
	std::string script = joinPaths(location.getRoot(), request.path);
	Logger::info("CgiPollable::start() → script: " + script);

	// 2) Access
	if (::access(script.c_str(), R_OK) != 0)
	{
		std::string rootAndScript = joinRootAndScript(location.getRoot(), script);
		Logger::info("CgiPollable::start() → Mending scrip path: " + script + " → " + rootAndScript);
		if (::access(rootAndScript.c_str(), R_OK) != 0)
			return (false);
		script = rootAndScript;
	}
	if (::access(cgiConfig.executable_path.c_str(), X_OK) != 0)
		return (false);

	// 3) Pipe
	int inPipe[2], outPipe[2];
	if (pipe(inPipe) < 0 || pipe(outPipe) < 0)
		return (false);

	// 4) Env / Argv
	std::vector<std::string> envStore = buildEnv(request, script);
	std::vector<char *> envPtrs;
	char **envp = toEnvp(envStore, envPtrs);

	std::vector<std::string> argvStore;
	std::vector<char *> argvPtrs;
	char **argv = makeArgv(cgiConfig.executable_path, script, argvStore, argvPtrs);

	// 5) Fork
	pid = fork();
	if (pid < 0)
	{
		::close(inPipe[0]);
		::close(inPipe[1]);
		::close(outPipe[0]);
		::close(outPipe[1]);
		return (false);
	}

	// ============== CHILD ==============
	if (pid == 0)
	{
		::close(inPipe[IO_WRITE]); // Close parent write end
		::close(outPipe[IO_READ]); // Close parent read end

		// Working directory
		std::string working_dir = script.substr(0, script.find_last_of('/'));
		if (working_dir[0] == '.')
			working_dir.erase(0, 1);

		// Navigate to working directory
		::chdir(working_dir.c_str());

		::dup2(inPipe[IO_READ], STDIN_FILENO);
		::dup2(outPipe[IO_WRITE], STDOUT_FILENO);

		::close(inPipe[IO_READ]);
		::close(outPipe[IO_WRITE]);

		// Execute
		::execve(cgiConfig.executable_path.c_str(), argv, envp);
		_exit(127); // Execve failed
	}

	// ============== PARENT ==============
	::close(inPipe[IO_READ]);	// Parent keeps write end only
	::close(outPipe[IO_WRITE]); // Parent keeps read end only

	stdin_fd = inPipe[IO_WRITE];
	stdout_fd = outPipe[IO_READ];

	setNonBlocking(stdin_fd);
	setNonBlocking(stdout_fd);

	// Queue body
	const bool has_body = (request.method == HttpMethod::POST && !request.body.empty());
	if (has_body)
		in_buf.assign(request.body.begin(), request.body.end());
	else
	{
		::close(stdin_fd);
		stdin_fd = -1;
		stdin_closed = true;
	}

	registerPollables(has_body);
	return (true);
}

// POLL REGISTRATION
// ____________________________________________________________________________
void CgiPollable::registerPollables(bool has_body)
{
	stdout_poll = std::make_unique<CgiStdoutPollable>(*this);
	if (loop)
		loop->add(stdout_poll.get());
	stdout_reg_fd = stdout_poll->fd();

	if (has_body)
	{
		stdin_poll = std::make_unique<CgiStdinPollable>(*this);
		if (loop)
			loop->add(stdin_poll.get());
		stdin_reg_fd = stdin_poll->fd();
	}
}

void CgiPollable::unregisterPollables()
{
	if (stdout_reg_fd >= 0)
	{
		loop->remove(stdout_reg_fd);
		stdout_reg_fd = -1;
	}
	if (stdin_reg_fd >= 0)
	{
		loop->remove(stdin_reg_fd);
		stdin_reg_fd = -1;
	}
}

// I/O
// ____________________________________________________________________________
void CgiPollable::readFromChild()
{
	// Client is gone
	if (!ownerConnection)
		return;

	if (stdout_fd < 0)
		return;

	char buf[4096];
	for (;;)
	{
		ssize_t n = ::read(stdout_fd, buf, sizeof(buf));
		if (n > 0)
		{
			out_buf.append(buf, static_cast<size_t>(n));
			continue;
		}
		if (n == 0)
		{
			// Child finished → close
			removeStdoutFromLoop();
			Logger::info("CgiProcess::readFromChild() → Closing fd: " + std::to_string(stdout_poll.get()->fd()));
			loop->closeLater(stdout_poll.get());
			stdout_fd = -1;
			stdout_closed = true;
			maybeFinish();
			return;
		}

		// (n < 0) → No Progress
		return;
	}
}

void CgiPollable::writeToChild()
{
	if (stdin_fd < 0)
		return;

	while (in_buf.empty() == false)
	{
		ssize_t n = ::write(stdin_fd, in_buf.data(), in_buf.size());
		if (n > 0)
		{
			in_buf.erase(0, static_cast<size_t>(n));
			continue;
		}
		if (n == 0)
		{
			// No progress → Stop for now
			return;
		}

		// (n < 0) → Child not accepting more
		Logger::info("CgiProcess::writeToChild() → Closing fd: " + std::to_string(stdin_poll.get()->fd()));
		removeStdinFromLoop();
		loop->closeLater(stdin_poll.get());
		stdin_fd = -1;
		stdin_closed = true;
		maybeFinish();
		return;
	}

	// Finished Sending
	removeStdinFromLoop();
	Logger::info("CgiProcess::writeToChild() → Closing fd: " + std::to_string(stdin_poll.get()->fd()));
	loop->closeLater(stdin_poll.get());
	stdin_fd = -1;
	stdin_closed = true;
	maybeFinish();
}

void CgiPollable::stdoutHangup(short /*revents*/)
{
	if (stdout_fd >= 0)
	{
		removeStdoutFromLoop();
		Logger::info("CgiProcess::stdoutHangup() → Closing fd: " + std::to_string(stdout_poll.get()->fd()));
		loop->closeLater(stdout_poll.get());
		stdout_fd = -1;
	}
	stdout_closed = true;
	maybeFinish();
}

void CgiPollable::stdinHangup(short /*revents*/)
{
	if (stdin_fd >= 0)
	{
		removeStdinFromLoop();
		Logger::info("CgiProcess::stdinHangup() → Closing fd: " + std::to_string(stdin_poll.get()->fd()));
		loop->closeLater(stdin_poll.get());
		stdin_fd = -1;
	}
	stdin_closed = true;
	maybeFinish();
}

// DONE
// ____________________________________________________________________________
bool CgiPollable::extractAndWaitChild(int &status)
{
	if (pid <= 0)
	{
		// Nothing to extract
		return (true);
	}
	pid_t running = ::waitpid(pid, &status, WNOHANG);
	if (running == 0)
	{
		// Still running
		return (false);
	}
	if (running == pid || running == -1)
	{
		pid = -1;
		return (true);
	}
	return (false);
}

void CgiPollable::maybeFinish()
{
	// Done if both sides are closed
	if (!(stdin_closed && stdout_closed))
		return;

	int status = 0;
	(void)extractAndWaitChild(status);

	if (!ownerConnection)
	{
		unregisterPollables();
		return;
	}

	// Build response
	makeCgiResponse();
	unregisterPollables();
}

void CgiPollable::clientAborted()
{
	Logger::info("CgiPollable::clientAborted() → Closing IN/OUT");
	ownerConnection = NULL;

	if (stdin_fd >= 0)
	{
		Logger::info("CgiProcess::clientAborted() → Closing fd: " + std::to_string(stdin_poll.get()->fd()));
		loop->closeLater(stdin_poll.get());
		stdin_fd = -1;
	}
	if (stdout_fd >= 0)
	{
		Logger::info("CgiProcess::clientAborted() → Closing fd: " + std::to_string(stdout_poll.get()->fd()));
		loop->closeLater(stdout_poll.get());
		stdout_fd = -1;
	}
	terminateChild();
}

void CgiPollable::terminateChild()
{
	if (pid > 0)
	{
		Logger::info("CgiPollable::terminateChild() → Killing process: " + std::to_string(pid));
		::kill(pid, SIGTERM);
	}
}

void CgiPollable::makeCgiResponse()
{
	// Cliend aborted
	if (!ownerConnection)
		return;

	HttpResponse response;

	auto [headers, body_indix] = parseCgiHeaders(out_buf);

	response.httpStatus = HttpStatus::STATUS_OK;
	if (auto it = headers.find("Status"); it != headers.end())
	{
		int code = 200;
		std::istringstream iss(it->second);
		if (iss >> code)
			response.httpStatus = toHttpStatus(code);
	}

	if (auto it = headers.find("Content-Type"); it != headers.end())
		response.headers[KEY_CONTENT_TYPE] = it->second;
	else
		response.headers[KEY_CONTENT_TYPE] = "text/plain; charset=utf-8";

	response.body.assign(out_buf, body_indix, std::string::npos);
	response.headers[KEY_CONTENT_LENGTH] = std::to_string(response.body.size());

	ownerConnection->setResponseAndSwitchToWriting(std::move(response));

	if (loop)
		loop->update(ownerConnection);
}
