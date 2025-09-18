/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:19:51 by quentinbeuk       #+#    #+#             */
/*   Updated: 2025/09/15 11:53:10 by qbeukelm         ###   ########.fr       */
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
		const char *window = inBuf.data() + offset;
		size_t window_size = inBuf.size() - offset;

		ParseStep step = parser.step(parseContext, window, window_size);
		offset += step.consumed;

		// 1) Handle parse error
		if (step.status != PARSE_MALFORMED_REQUEST || step.status != PARSE_INCOMPLETE)
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

		// 3) Need more bytes to proceed → stop feeding for now
		if (step.need_more)
			break;
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
	char buf[8192]; // 8kb

	while (true)
	{
		ssize_t n = ::recv(fd_, buf, sizeof(buf), 0);
		if (n > 0)
		{
			lastActivityMs = loop->nowMs();
			inBuf.append(buf, static_cast<size_t>(n));
		}
		else if (n == 0)
		{
			// Peer closed
			keepAlive = false;
			loop->closeLater(this);
			return;
		}
		else
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break; // No more data to read now
			if (errno == EINTR)
				continue; // Retry read
			// Fatal read error
			loop->closeLater(this);
			return;
		}
	}

	feedParserFromBuffer();
}

void Connection::onWritable()
{
	// TODO: Connection::onWritable()
}

/*
 * Use `outBuf` as a "write flag"
 * 	- if `outBuf.empty()`, the connection has nothing to send → Only interested in POLLIN.
 * 	- if `!outBuf.empty()`, there is pending data to send → also interested in POLLOUT.
 */
short Connection::interest() const
{
	return (outBuf.empty() ? POLLIN : POLLIN | POLLOUT);
}

void Connection::onHangupOrError(short revents)
{
	// TODO: Connection::onHangupOrError()
	Logger::error("Connection::onHangupOrError → " + std::to_string(revents));
}
