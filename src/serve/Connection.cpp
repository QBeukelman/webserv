/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 16:19:51 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 16:13:01 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/Connection.hpp"

// CONSTRUCTOR
// ____________________________________________________________________________
Connection::Connection(int clientFd, const Server *server, EventLoop *loop, unsigned short port)
	: fd_(clientFd), port(port), parser(parse_context.limits), keep_alive(false), loop(loop), server(server),
	  last_activity(std::chrono::steady_clock::now()), keep_alive_pending(false)
{
	Logger::info("Connection::Connection(" + std::to_string(clientFd) + ")");

	// Init ParseContext
	parse_context = ParseContext();
	connection_state = ConnectionState::READING;

	// Buffers start empty
	inBuf.clear();
	outBuf.clear();
}

int Connection::fd() const
{
	return (this->fd_);
}

// GETTERS & SETTERS
// ____________________________________________________________________________
unsigned short Connection::getPort() const
{
	return (this->port);
}

ConnectionState Connection::getConnectionState() const
{
	return (this->connection_state);
}

bool Connection::getKeepAlivePending() const
{
	return (this->keep_alive_pending);
}

void Connection::setCgi(std::unique_ptr<CgiProcess> new_cgi)
{
	cgi_ = std::move(new_cgi);
}

bool Connection::isClosing() const
{
	return (connection_state == ConnectionState::CLOSING);
}

// PARSER
// ____________________________________________________________________________
static bool parserStalled(const ParseStep &step)
{
	if (step.consumed == 0 && !step.need_more && !step.request_complete && step.status != PARSE_INVALID_METHOD &&
		step.status != PARSE_INVALID_VERSION && step.status != PARSE_MALFORMED_REQUEST &&
		step.status != PARSE_EXCEED_BODY_LIMIT)
	{
		return (true);
	}
	return (false);
}

bool Connection::handleParseError(const ParseStep &step)
{
	if (step.status != PARSE_INCOMPLETE && step.status != PARSE_OK)
	{
		HttpStatus httpStatus = HttpStatus::STATUS_BAD_REQUEST;
		if (step.status == PARSE_EXCEED_BODY_LIMIT)
			httpStatus = HttpStatus::STATUS_PAYLOAD_TOO_LARGE;
		if (step.status == PARSE_EXCEED_STARTLINE_LIMIT)
			httpStatus = HttpStatus::STATUS_URI_TOO_LARGE;
		if (step.status == PARSE_INVALID_METHOD)
			httpStatus = HttpStatus::STATUS_METHOD_NOT_ALLOWED;
		if (step.status == PARSE_INVALID_VERSION)
			httpStatus = HttpStatus::STATUS_HTTP_VERSION_NOT_SUPPORTED;

		HttpResponse response = RequestHandler(*this->server).makeError(httpStatus, reasonPhrase(httpStatus));
		outBuf = response.serialize();
		keep_alive = false;
		loop->update(this); // POLLOUT

		return (true);
	}
	return (false);
}

// TIME OUT
// ____________________________________________________________________________
void Connection::updateActivityNow(void)
{
	this->last_activity = std::chrono::steady_clock::now();
}

static std::chrono::milliseconds phaseLimitMs(const Connection &connection)
{
	if (connection.getConnectionState() == ConnectionState::READING && !connection.getKeepAlivePending())
		return std::chrono::milliseconds(READ_IDLE_MS);
	if (connection.getConnectionState() == ConnectionState::WRITING)
		return std::chrono::milliseconds(WRITE_IDLE_MS);
	if (connection.getConnectionState() == ConnectionState::READING && connection.getKeepAlivePending())
		return std::chrono::milliseconds(KEEPALIVE_IDLE_MS);
	return (std::chrono::milliseconds(READ_IDLE_MS));
}

/*
 * Given the current time, how many milliseconds remain before this connection should timeout.
 */
int Connection::remainingMsUntilTimeout(std::chrono::steady_clock::time_point now) const
{
	std::chrono::milliseconds limit = phaseLimitMs(*this);
	std::chrono::steady_clock::time_point deadline = this->last_activity + limit;

	if (now >= deadline)
		return (0);

	std::chrono::steady_clock::duration remaining = deadline - now;
	return (static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(remaining).count()));
}

bool Connection::hasTimedOut(std::chrono::steady_clock::time_point now) const
{
	return (this->remainingMsUntilTimeout(now) == 0);
}

int Connection::timeBudgetMs(std::chrono::steady_clock::time_point now) const
{
	return (this->remainingMsUntilTimeout(now));
}

void Connection::onTimeout()
{
	// 1) Client started sending request, but never finished
	if (connection_state == ConnectionState::READING && !keep_alive_pending)
	{
		if (cgi_)
			cgi_->clientAborted();

		// Request not fully received
		HttpResponse response = RequestHandler(*server).makeError(STATUS_REQUEST_TIMEOUT, "Request timeout");
		outBuf = response.serialize();
		keep_alive = false;
		connection_state = ConnectionState::WRITING;
		loop->update(this);
		return;
	}

	// 2) Idle between requests
	if (connection_state == ConnectionState::READING && keep_alive_pending)
	{
		if (cgi_)
			cgi_->clientAborted();

		keep_alive = false;
		connection_state = ConnectionState::CLOSING;
		loop->closeLater(this);
		Logger::info("Connection::onTimeout() → Closing fd: " + std::to_string(this->fd()));
		return;
	}

	// 3) Timeout while writing response
	if (connection_state == ConnectionState::WRITING)
	{
		if (cgi_)
			cgi_->clientAborted();

		keep_alive = false;
		connection_state = ConnectionState::CLOSING;
		loop->closeLater(this);
		Logger::info("Connection::onTimeout() → Closing fd: " + std::to_string(this->fd()));
		return;
	}
}

// PARSE
// ____________________________________________________________________________
void Connection::feedParserFromBuffer()
{
	while (parse_context.read_offset < inBuf.size())
	{
		// 1) Step function
		size_t before = parse_context.read_offset;
		ParseStep step = parser.step(parse_context, inBuf.data(), inBuf.size());
		if (parserStalled(step) == true)
			break;

		Logger::info("Connection::feedParserFromBuffer() → Feeding step, read (" +
					 std::to_string(parse_context.read_offset) + "/" + std::to_string(inBuf.size()) + ")");

		// 2) Handle parse error
		if (handleParseError(step))
		{
			Logger::info("Connection::feedParserFromBuffer() → Will handle parse error");

			if (parse_context.read_offset > 0)
			{
				inBuf.erase(0, parse_context.read_offset);
				parse_context.read_offset = 0;
			}
			connection_state = ConnectionState::WRITING;
			loop->update(this); // POLLOUT
			return;
		}

		// 3) Complete request
		if (step.request_complete)
		{
			Logger::info("Connection::feedParserFromBuffer() → Step Complete!");
			DispatchResult dispatch = RequestHandler(*server).dispatch(parse_context.request);

			if (dispatch.kind == dispatch.DISPACTH_STATIC)
			{
				Logger::info("Connection::feedParserFromBuffer() → Dispatch STATIC");
				outBuf = dispatch.response.serialize();

				if (dispatch.response.searchHeader("Content-Length").empty())
				{
					Logger::info(
						"Connection::feedParserFromBuffer() → Response missing [Content-Length]. Keep alive false");
					keep_alive = false;
				}
				else
					keep_alive = true;

				// Reset parser
				if (parse_context.read_offset > 0)
				{
					inBuf.erase(0, parse_context.read_offset);
					parse_context.read_offset = 0;
				}

				connection_state = ConnectionState::WRITING;
				loop->update(this); // POLLOUT
				return;
			}
			if (dispatch.kind == dispatch.DISPACTH_CGI)
			{
				Logger::info("Connection::feedParserFromBuffer() → Dispatch CGI");

				auto cgi =
					std::make_unique<CgiProcess>(loop, this, parse_context.request, dispatch.location, dispatch.cgi);
				if (!cgi->start())
				{
					Logger::error("Connection::feedParserFromBuffer() → CGI failed to start [500]");
					continue;
				}
				this->setCgi(std::move(cgi));
			}
		}

		// 3) Need more bytes to proceed → Return to poll() for more bytes
		if (step.need_more)
		{
			if (parse_context.read_offset == before || parse_context.read_offset >= inBuf.size())
			{
				Logger::info("Connection::feedParserFromBuffer() → Need more bytes from recv(), breaking");
				break;
			}
			continue;
		}
	}
}

// PUBLIC
// ____________________________________________________________________________
/*
 * A client socket is marked as readble when a client has sent data.
 * (or closed a connection).
 *
 * `onReadable()` calles `recv()` and feeds bytes to HttpParser.
 *
 * Notes:
 * 	- Readable = client sent data.
 * 	- `16kb` buffer.
 */
void Connection::onReadable()
{
	if (isClosing())
		return;
	if (connection_state != ConnectionState::READING)
		return;

	static const size_t buf_size = 16 * 1024;
	char buf[buf_size];

	for (;;)
	{
		ssize_t n = ::recv(fd_, buf, sizeof(buf), 0);
		if (n > 0)
		{
			this->updateActivityNow();

			inBuf.append(buf, static_cast<size_t>(n));
			feedParserFromBuffer();
			return;
		}
		else if (n == 0)
		{
			// Peer closed
			keep_alive = false;
			if (cgi_)
				cgi_->clientAborted();

			connection_state = ConnectionState::CLOSING;
			Logger::info("Connection::onReadable() → Closing fd: " + std::to_string(this->fd()));
			loop->closeLater(this);
			return;
		}

		// n < 0
		connection_state = ConnectionState::CLOSING;
		Logger::info("Connection::onReadable() → Closing fd: " + std::to_string(this->fd()));
		loop->closeLater(this);
		Logger::info("Connection::onReadable() → Reach end of recv");
		return;
	}
}

void Connection::onWritable()
{
	if (isClosing())
		return;
	if (connection_state != ConnectionState::WRITING)
		return;

	if (outBuf.empty())
	{
		if (keep_alive)
		{
			// Nothing to write, switch phase
			keep_alive_pending = true;
			connection_state = ConnectionState::READING;
			this->updateActivityNow();
			loop->update(this);
			return;
		}
		else
		{
			connection_state = ConnectionState::CLOSING;
			Logger::info("Connection::onWritable() → Closing fd: " + std::to_string(this->fd()));
			loop->closeLater(this);
		}
	}

	ssize_t n = ::send(fd_, outBuf.data(), outBuf.size(), 0);
	if (n > 0)
	{
		this->updateActivityNow();

		outBuf.erase(0, n);
		if (outBuf.empty())
		{
			// Response complete
			std::cout << LIGHT_GRAY << "====================================================\n" << RESET_STYLE;
			Logger::success("Response sent");
			std::cout << LIGHT_GRAY << "====================================================\n" << RESET_STYLE;

			if (keep_alive == false)
			{
				connection_state = ConnectionState::CLOSING;
				Logger::info("Connection::onWritable() → Closing fd: " + std::to_string(this->fd()));
				loop->closeLater(this);
				return;
			}
			if (keep_alive == true)
			{
				// Reset connection
				parse_context = ParseContext();
				keep_alive_pending = true;
				connection_state = ConnectionState::READING;
				this->updateActivityNow();
				loop->update(this);
			}
		}
	}

	if (n <= 0)
	{
		// Write error → Close
		connection_state = ConnectionState::CLOSING;
		loop->closeLater(this);
		return;
	}
}

/*
 * Use `outBuf` as a "write flag"
 * 	- if `outBuf.empty()`, the connection has nothing to send → Only interested in POLLIN.
 * 	- if `!outBuf.empty()`, there is pending data to send → also interested in POLLOUT.
 */
short Connection::interest() const
{
	short interest = 0;

	switch (connection_state)
	{
	case ConnectionState::READING: {
		Logger::info("Connection::interest() → New interest: POLLIN");
		return (POLLIN);
	}
	case ConnectionState::WRITING: {
		Logger::info("Connection::interest() → New interest: POLLOUT");
		return (POLLOUT);
	}
	case ConnectionState::CLOSING: {
		Logger::info("Connection::interest() → New interest: CLOSING");
		return (0);
	}
	}
	return (0);
}

void Connection::onHangupOrError(short revents)
{
	if (isClosing())
		return;
	if (cgi_)
		cgi_->clientAborted();

	if (revents & POLLHUP)
	{
		Logger::info("Connection::onHangupOrError() → POLLHUP (peer closed)");
		if (parse_context.mismatch_body_len == true)
		{
			HttpResponse response =
				RequestHandler(*server).makeError(HttpStatus::STATUS_BAD_REQUEST, "Invalid content length");
			outBuf = response.serialize();
			keep_alive = false;
			connection_state = ConnectionState::WRITING;
			loop->update(this);
		}
		if (outBuf.empty())
		{
			// connection_state = ConnectionState::WRITING;
			connection_state = ConnectionState::CLOSING;
			this->interest();
			Logger::info("Connection::onHangupOrError() → Closing fd: " + std::to_string(this->fd()));
			loop->closeLater(this);
		}
		return;
	}

	if (revents & POLLERR)
	{
		int socket_error = 0;
		socklen_t sl = sizeof(socket_error);
		if (::getsockopt(fd(), SOL_SOCKET, SO_ERROR, &socket_error, &sl) == 0 && socket_error != 0)
		{
			Logger::error("Connection::onHangupOrError() → POLLERR: " + std::string(std::strerror(socket_error)));
		}
		else
		{
			Logger::error("Connection::onHangupOrError() → POLLERR (unknown)");
		}
	}

	if (revents & POLLNVAL)
	{
		// Fd was invalid for polling
		Logger::info("Connection::onHangupOrError() → Closing fd: " + std::to_string(this->fd()));
		loop->closeLater(this);
		return;
	}
}

void Connection::setResponseAndSwitchToWriting(HttpResponse &&response)
{
	// prepareKeepAliveHeaders(response);
	outBuf = response.serialize();
	connection_state = ConnectionState::WRITING;

	if (loop)
		loop->update(this);
}
