/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleCgi.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/27 16:38:54 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/01 11:20:52 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

enum IO
{
	IO_READ,
	IO_WRITE
};

// RESPONSES
// ____________________________________________________________________________
static HttpResponse makeError(HttpStatus status, const std::string log_message)
{
	Logger::error("RequestHandler::handleCgi() → Could not access cgi file: " + log_message);

	HttpResponse response;
	response.httpStatus = STATUS_FOUND;
	return (response);
}

static HttpResponse makeCgiResponse(const std::string &raw)
{
	// Split Headers / Body
	size_t pos = raw.find("\r\n\r\n");
	std::string head = (pos == std::string::npos) ? raw : raw.substr(0, pos);
	std::string body = (pos == std::string::npos) ? "" : raw.substr(pos + 4);

	HttpResponse response;
	response.httpStatus = HttpStatus::STATUS_OK;

	std::istringstream h(head);
	std::string line;
	while (std::getline(h, line))
	{
		if (line.empty() == false && line.back() == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			break;

		size_t c = line.find(':');
		if (c == std::string::npos)
			continue;

		std::string key = line.substr(0, c);
		std::string value = line.substr(c + 1);

		if (value.empty() == false && value[0] == ' ')
			value.erase(0, 1);

		if (key == "Status")
			response.httpStatus = toHttpStatus(std::atoi(value.c_str()));
		else
			response.headers[key] = value;
	}

	response.body = body;
	if (response.headers.find(KEY_CONTENT_LENGTH) == response.headers.end())
		response.headers[KEY_CONTENT_LENGTH] = std::to_string(body.size());
	if (response.headers.find(KEY_CONTENT_TYPE) == response.headers.end())
		response.headers[KEY_CONTENT_TYPE] = "text/html";

	return (response);
}

// HELPERS
// ____________________________________________________________________________
static char **toEnvp(const std::vector<std::string> &envStore, std::vector<char *> &envPtrs)
{
	envPtrs.clear();

	for (size_t i = 0; i < envStore.size(); ++i)
		envPtrs.push_back(const_cast<char *>(envStore[i].c_str()));
	envPtrs.push_back(NULL);

	return (&envPtrs[0]);
}

static void pushEnv(std::vector<std::string> &env, const std::string &key, const std::string &value)
{
	std::string new_entry = key + "=" + value;
	env.push_back(new_entry);
}

static std::vector<std::string> buildEnv(const HttpRequest &request, const std::string &script)
{
	std::vector<std::string> env;
	pushEnv(env, "GATEWAY_INTERFACE", "CGI/1.1");
	pushEnv(env, "REQUEST_METHOD", toStringMethod(request.method));
	pushEnv(env, "QUERY_STRING", request.query);
	pushEnv(env, "SERVER_PROTOCOL", request.version);
	pushEnv(env, "SCRIPT_FILENAME", script);
	pushEnv(env, "SCRIPT_NAME", request.path);

	if (request.searchHeader(KEY_CONTENT_TYPE).empty() == false)
		pushEnv(env, "CONTENT_TYPE", request.searchHeader(KEY_CONTENT_TYPE));
	if (request.body.empty() == false)
		pushEnv(env, "CONTENT_LENGTH", std::to_string(request.body.size()));

	std::string serverName;
	std::string hostName = request.getHostName();

	if (hostName.empty() == false)
		serverName = hostName;
	else
		serverName = "127.0.0.1"; // Fallback

	pushEnv(env, "SERVER_NAME", serverName);
	pushEnv(env, "SERVER_PORT", std::to_string(request.getHostPort()));

	return (env);
}

static char **makeArgv(const std::string &exc_path, const std::string &script, std::vector<std::string> &store,
					   std::vector<char *> &ptrs)
{
	store.clear();
	ptrs.clear();

	store.push_back(exc_path);
	store.push_back(script);

	for (size_t i = 0; i < store.size(); ++i)
		ptrs.push_back(const_cast<char *>(store[i].c_str()));
	ptrs.push_back(NULL);

	return (&ptrs[0]);
}

// CGI
// ____________________________________________________________________________
HttpResponse RequestHandler::handleCgi(const HttpRequest &request, const Location &location, const CGI &cgi) const
{

	// 1) Resolve paths
	const std::string root = location.normalizeDirectory(location.getRoot());
	const std::string script = root + request.path;

	// 2) Check access
	if (::access(script.c_str(), R_OK) != 0)
		return (makeError(STATUS_FOUND, "RequestHandler::handleCgi() → Could not access cgi file: " + script));

	// 3) Choose working directory
	const std::string working_dir = cgi.working_directory.empty() ? root : cgi.working_directory;

	// 4) Create pipes
	int inPipe[2], outPipe[2];
	if (pipe(inPipe) < 0 || pipe(outPipe) < 0)
		return (makeError(STATUS_INTERNAL_ERROR, "RequestHandler::handleCgi() → Faild to pipe()"));

	// 5) Build Env & Argv
	std::vector<std::string> envStore = buildEnv(request, script);
	std::vector<char *> envPtrs;
	char **envp = toEnvp(envStore, envPtrs);

	std::vector<std::string> argvStore;
	std::vector<char *> argvPtrs;
	char **argv = makeArgv(cgi.executable_path, script, argvStore, argvPtrs);

	// 6) Fork
	pid_t pid = fork();
	if (pid < 0)
		return (makeError(STATUS_INTERNAL_ERROR, "RequestHandler::handleCgi() → Failed to fork()"));

	// CHILD
	// _________________________________
	if (pid == 0)
	{
		::close(inPipe[IO_WRITE]); // Close parent write-end
		::close(outPipe[IO_READ]); // Close parent read-end

		::chdir(working_dir.c_str()); // Navigate to working-directory

		::dup2(inPipe[IO_READ], IO_READ);
		::dup2(outPipe[IO_WRITE], IO_WRITE);

		::close(inPipe[IO_READ]);
		::close(outPipe[IO_WRITE]);

		// 7) Execute
		::execve(cgi.executable_path.c_str(), argv, envp);
	}

	// PARENT
	// _________________________________
	::close(inPipe[IO_READ]);
	::close(outPipe[IO_WRITE]);

	// 8) Give body to execve()
	if (request.method == HttpMethod::POST)
	{
		if (request.body.empty() == false)
			::write(inPipe[1], request.body.data(), request.body.size());
	}
	::close(inPipe[IO_WRITE]);

	// 9) Read all CGI output
	std::string raw;
	char buf[4096];
	for (;;)
	{
		ssize_t n = ::read(outPipe[IO_READ], buf, sizeof(buf));

		if (n > 0)
			raw.append(buf, n);
		else
			break;
	}
	::close(outPipe[IO_READ]);

	return (makeCgiResponse(raw));
}
