/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 14:27:52 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/27 10:27:14 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "http/RequestHandler.hpp"
#include "http/RequestParser.hpp"
#include "http/models/HttpResponse.hpp"
#include "http/models/ParseContext.hpp"
#include "log/Logger.hpp"
#include "serve/EventLoop.hpp"
#include "serve/IOPollable.hpp"

#include <cstring>
#include <string>
#include <sys/socket.h>

class Server;	 // Forward
class EventLoop; // Forward

enum class ConnectionState
{
	READING,
	WRITING,
	CLOSING
};

/*
 * A `Connection` represents an active TCP session with a single client.
 * It holds:
 *
 * 		- Client fd's from `accept()`.
 * 		- Input buffer and `ParseContext()`.
 * 		- Flags like `keep-alive` and timeouts, etc.
 */
class Connection : public IOPollable
{
  private:
	int fd_;

	ParseContext parse_context;
	RequestParser parser;
	ConnectionState connection_state;

	std::string inBuf;
	std::string outBuf;

	bool keepAlive;
	unsigned long lastActivityMs;

	const Server *server; // Which server this listener serves
	EventLoop *loop;	  // To register new connections

	// Parse
	bool handleParseError(const ParseStep &step);
	void feedParserFromBuffer();

  public:
	Connection(int clientFd, const Server *server, EventLoop *loop);

	// IOPollable
	int fd() const;
	short interest() const;
	void onReadable();
	void onWritable();
	void onHangupOrError(short revents);
};

#endif
