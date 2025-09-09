/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 16:19:51 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/09 17:19:08 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/Connection.hpp"

// CONSTRUCTOR
// ____________________________________________________________________________
Connection::Connection(int clientFd)
{
	Logger::info("Connection::Connection(" + std::to_string(clientFd) + ")");
}

int Connection::fd()
{
	return (this->fd_);
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
	// TODO: Connection::onReadable()
}

void Connection::onWritable()
{
	// TODO: Connection::onWritable()
}

void Connection::onHangupOrError(short revents)
{
	// TODO: Connection::onHangupOrError()
	Logger::error("Connection::onHangupOrError → " + std::to_string(revents));
}
