/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 14:27:52 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 23:40:02 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "http/RequestHandler.hpp"
#include "http/RequestParser.hpp"
#include "http/models/HttpResponse.hpp"
#include "http/models/ParseContext.hpp"
#include "log/Logger.hpp"
#include "serve/CgiPollable.hpp"
#include "serve/EventLoop.hpp"
#include "serve/IOPollable.hpp"

#include <chrono>
#include <cstring>
#include <memory>
#include <string>
#include <sys/socket.h>

#define READ_IDLE_MS 5000
#define WRITE_IDLE_MS 5000
#define KEEPALIVE_IDLE_MS 5000

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
	std::unique_ptr<CgiPollable> cgi_;

	ParseContext parse_context;
	RequestParser parser;
	ConnectionState connection_state;

	std::string inBuf;
	std::string outBuf;

	bool keep_alive;
	unsigned short port;

	const Server *server; // Which server this listener serves
	EventLoop *loop;	  // To register new connections

	// Timeout
	std::chrono::steady_clock::time_point last_activity;
	bool keep_alive_pending;

	void updateActivityNow(void);
	int remainingMsUntilTimeout(std::chrono::steady_clock::time_point now) const;
	bool hasTimedOut(std::chrono::steady_clock::time_point now) const;

	// Parse
	bool handleParseError(const ParseStep &step);
	void feedParserFromBuffer();

  public:
	Connection(int clientFd, const Server *server, EventLoop *loop, unsigned short port);

	// Getters & Setters
	unsigned short getPort() const;
	ConnectionState getConnectionState() const;
	bool getKeepAlivePending() const;
	void setCgi(std::unique_ptr<CgiPollable> new_cgi);
	bool isClosing() const;

	// Timeout
	int timeBudgetMs(std::chrono::steady_clock::time_point now) const;
	void onTimeout();

	// Make CGI response
	void setResponseAndSwitchToWriting(HttpResponse &&);

	// IOPollable
	int fd() const;
	short interest() const;
	void onReadable();
	void onWritable();
	void onHangupOrError(short revents);
};

#endif
