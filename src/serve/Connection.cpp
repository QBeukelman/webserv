/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 16:19:51 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/27 22:06:53 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/Connection.hpp"

// CONSTRUCTOR
// ____________________________________________________________________________
Connection::Connection(int clientFd, const Server *server, EventLoop *loop, unsigned short port)
	: fd_(clientFd), port(port), parser(parse_context.limits), keepAlive(false), lastActivityMs(0), loop(loop),
	  server(server)
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

unsigned short Connection::getPort() const
{
	return (this->port);
}

static bool parserStalled(const ParseStep &step)
{
	if (step.consumed == 0 && !step.need_more && !step.request_complete && step.status != PARSE_INVALID_METHOD &&
		step.status != PARSE_INVALID_VERSION && step.status != PARSE_MALFORMED_REQUEST &&
		step.status != PARSE_EXCEED_LIMIT)
	{
		return (true);
	}
	return (false);
}

bool Connection::handleParseError(const ParseStep &step)
{
	if (step.status == PARSE_INVALID_METHOD || step.status == PARSE_INVALID_VERSION ||
		step.status == PARSE_MALFORMED_REQUEST || step.status == PARSE_EXCEED_LIMIT)
	{
		HttpStatus st = (step.status == PARSE_EXCEED_LIMIT) ? HttpStatus::STATUS_PAYLOAD_TOO_LARGE // 413
															: HttpStatus::STATUS_BAD_REQUEST;	   // 400

		HttpResponse response = RequestHandler(*this->server).makeError(st, "Bad request");
		outBuf = response.serialize();
		keepAlive = false;
		loop->update(this); // Switch to POLLOUT
		return (true);
	}
	return (false);
}

// PARSE
// ____________________________________________________________________________
void Connection::feedParserFromBuffer()
{
	while (parse_context.read_offset < inBuf.size())
	{
		// 1) Step function
		ParseStep step = parser.step(parse_context, inBuf.data(), inBuf.size());
		if (parserStalled(step) == true)
			break;

		Logger::info("Connection::feedParserFromBuffer() → Step: " + toStringStatus(step.status));

		// 2) Handle parse error
		if (handleParseError(step))
		{
			// Optional: compact what was consumed
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
			HttpResponse response = RequestHandler(*server).handle(parse_context.request);
			outBuf = response.serialize();

			// TODO: Should keep alive?
			keepAlive = true;

			if (parse_context.read_offset > 0)
			{
				inBuf.erase(0, parse_context.read_offset);
				parse_context.read_offset = 0;
			}

			connection_state = ConnectionState::WRITING;
			loop->update(this); // POLLOUT
			return;
		}

		// 3) Need more bytes to proceed → Return to poll() for more bytes
		if (step.need_more)
			break;
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
	if (connection_state != ConnectionState::READING)
		return;

	static const size_t buf_size = 16 * 1024;
	char buf[buf_size];

	for (;;)
	{
		ssize_t n = ::recv(fd_, buf, sizeof(buf), 0);
		if (n > 0)
		{
			// TODO: Connection::onReadable() What is max time?
			lastActivityMs = loop->nowMs();
			inBuf.append(buf, static_cast<size_t>(n));
			feedParserFromBuffer();
			return;
		}
		else if (n == 0)
		{
			// Peer closed
			keepAlive = false;
			loop->closeLater(this);
			Logger::info("Connection::onReadable() → recv read 0 bytes, returning early");
			return;
		}

		// n < 0
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			Logger::error("Connection::onReadable() (errno == EAGAIN || errno == EWOULDBLOCK)");
			continue; // Retry recv()
		}
		if (errno == EINTR)
		{
			Logger::error("Connection::onReadable() (errno == EINTR)");
			return; // Nothing to read now, wait for next POLLIN
		}

		connection_state = ConnectionState::CLOSING;
		loop->closeLater(this);
		Logger::info("Connection::onReadable() → Reach end of recv");
		return;
	}
}

void Connection::onWritable()
{
	if (connection_state != ConnectionState::WRITING)
		return;

	if (outBuf.empty())
	{
		// Nothing to write
		loop->update(this);
		return;
	}

	ssize_t n = ::send(fd_, outBuf.data(), outBuf.size(), 0);
	if (n > 0)
	{
		outBuf.erase(0, n);
		if (outBuf.empty())
		{
			// Response complete
			if (keepAlive == false)
			{
				loop->closeLater(this);
				return;
			}

			// TODO: Reset parser
			// parser.reset();
			parse_context = ParseContext();
			connection_state = ConnectionState::READING;
			loop->update(this);
		}
	}
	else if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
	{
		// Keep POLLOUT, try onWritable again
	}
	else
	{
		// TODO: onWritable() Error → close connection
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
	connection_state = ConnectionState::CLOSING;
	if (revents & POLLHUP)
	{
		// Hang up
		Logger::info("Connection::onHangupOrError() → POLLHUP (peer closed)");
		loop->closeLater(this);
	}

	if (revents & POLLERR)
	{
		int soerr = 0;
		socklen_t sl = sizeof(soerr);
		if (::getsockopt(fd(), SOL_SOCKET, SO_ERROR, &soerr, &sl) == 0 && soerr != 0)
		{
			Logger::error("Connection::onHangupOrError() → POLLERR: " + std::string(std::strerror(soerr)));
		}
		else
		{
			Logger::error("Connection::onHangupOrError() → POLLERR (unknown)");
		}
	}

	if (revents & POLLNVAL)
	{
		// Fd was invalid for polling
		Logger::error("Connection::onHangupOrError() → POLLNVAL (invalid fd)");
		loop->closeLater(this);
		return;
	}
}
