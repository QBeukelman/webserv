/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 16:19:51 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/24 09:47:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/Connection.hpp"

// CONSTRUCTOR
// ____________________________________________________________________________
Connection::Connection(int clientFd, const Server *server, EventLoop *loop)
	: fd_(clientFd), parser(parseContext.limits), keepAlive(false), lastActivityMs(0), loop(loop), server(server)
{
	Logger::info("Connection::Connection(" + std::to_string(clientFd) + ")");

	// Init ParseContext
	parseContext = ParseContext();

	// Buffers start empty
	inBuf.clear();
	outBuf.clear();
}

int Connection::fd() const
{
	return (this->fd_);
}

// PARSE
// ____________________________________________________________________________
void Connection::feedParserFromBuffer()
{
	size_t offset = 0;

	while (offset < inBuf.size())
	{
		const char *data = inBuf.data();
		size_t window_size = inBuf.size() - offset;

		std::cout << "Data: " << data << std::endl;

		ParseStep step = parser.step(parseContext, data, window_size);
		offset += step.consumed;

		Logger::info("Connection::feedParserFromBuffer() → Step: " + toStringStatus(step.status));
		std::cout << step << std::endl;

		// 1) Handle parse error
		if (step.status == PARSE_INVALID_METHOD || step.status == PARSE_INVALID_VERSION ||
			step.status == PARSE_MALFORMED_REQUEST || step.status == PARSE_EXCEED_LIMIT)
		{
			std::string detail = "Bad request";
			HttpResponse response = RequestHandler(*this->server).makeError(HttpStatus::STATUS_BAD_REQUEST, detail);
			outBuf = response.serialize();
			keepAlive = false;
			break;
			loop->update(this); // Want POLLOUT
		}

		// 2) Complete request
		if (step.request_complete)
		{
			HttpResponse response = RequestHandler(*this->server).handle(parseContext.request);
			outBuf = response.serialize();

			// TODO: Connection::feedParserFromBuffer() → Keep-Alive decision
			keepAlive = true;

			loop->update(this); // Want POLLOUT

			// Prepare pipeline for next request → Leep leftover bytes
			if (offset < inBuf.size())
				inBuf.erase(0, offset);
			else
				inBuf.clear();

			parseContext = ParseContext(); // Reset state machine
			offset = 0;
			continue;
		}

		// TODO: Connection::feedParserFromBuffer() → Test with transfer encoding chunked
		// 3) Need more bytes to proceed → stop feeding for now
		// if (step.need_more)
		// 	break;
	}

	// Drop consumed bytes
	if (offset > 0 && offset <= inBuf.size())
		inBuf.erase(0, offset);
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
 * 	Readable = client sent data.
 */
void Connection::onReadable()
{
	// TODO: Connection::onReadable() what is buffer size?
	char buf[8192]; // 8kb

	ssize_t n = 0;

	n = ::recv(fd_, buf, sizeof(buf), 0);
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
		return;
	}
	if (errno == EINTR)
	{
		Logger::error("Connection::onReadable() (errno == EINTR)");
		return;
	}

	// TODO: Connection::onReadable() When should we mark this for closing?
	loop->closeLater(this);
	Logger::info("Connection::onReadable() → Reach end of recv");
	return;
}

void Connection::onWritable()
{
	// TODO: Connection::onWritable()
	ssize_t n = ::send(fd_, outBuf.data(), outBuf.size(), 0);
	if (n > 0)
	{
		outBuf.erase(0, n);
	}
	else if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
	{
		// Try again later when poll() says POLLOUT
	}
	else
	{
		// Error: close connection
	}
}

/*
 * Use `outBuf` as a "write flag"
 * 	- if `outBuf.empty()`, the connection has nothing to send → Only interested in POLLIN.
 * 	- if `!outBuf.empty()`, there is pending data to send → also interested in POLLOUT.
 */
short Connection::interest() const
{
	const short interest = outBuf.empty() ? POLLIN : POLLIN | POLLOUT;

	const std::string log = interest == POLLIN ? "POLLIN" : "POLLOUT";
	Logger::info("Connection::interest() → New interest: " + log);
	return (interest);
}

void Connection::onHangupOrError(short revents)
{
	// TODO: Connection::onHangupOrError()
	Logger::error("Connection::onHangupOrError → " + std::to_string(revents));
}
