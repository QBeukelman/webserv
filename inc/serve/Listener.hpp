/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Listener.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/04 09:13:07 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/14 02:10:33 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_HPP
#define LISTENER_HPP
#define UNIT_TEST

#include "config/ServerConfig.hpp"
#include "serve/Connection.hpp"
#include "serve/EventLoop.hpp"
#include "serve/IOPollable.hpp"

#include <cstring>
#include <sstream>
#include <stdexcept>

// POSIX / sockets
#include <arpa/inet.h>	// inet_pton, inet_ntop
#include <fcntl.h>		// fcntl(), O_NONBLOCK
#include <netinet/in.h> // sockaddr_in, htons(), htonl(), INADDR_ANY
#include <sys/poll.h>	// poll()
#include <sys/socket.h> // socket(), bind(), listen()
#include <sys/types.h>	// basic system types
#include <unistd.h>		// close()

/*
 * A `Listener` is a socket that is bound to an `ip:port`,
 * and put into `listen()` mode:
 *
 * 		- Wait for a new incoming TCP connections.
 * 		- When `poll()` says a listener fd is readable, we can `accept()`.
 * 		- After `accept()` we get a new fd. And that fd is crapped in a client `Connection`.
 */
class Listener : public IOPollable
{
  private:
	int fd_;			  // Listening socket fd
	const Server *server; // Which server this listener serves
	EventLoop *loop;	  // To register new connections

	void setReuseAddress();		 // SO_REUSEADDR
	void setNonBlocking(int fd); // O_NONBLOCK
	void bindAndListen(const std::string &ip, unsigned short port);
	void closeIfValid();
	unsigned short getLocalPort(int fd) const;

  public:
	explicit Listener(const std::string &ip, unsigned short port, const Server *server, EventLoop *loop);
	Listener(Listener &&other) noexcept;
	Listener &operator=(Listener &&other) noexcept;
	~Listener();

	// Listener is non-copyable & non-movable
	Listener(const Listener &) = delete;
	Listener &operator=(const Listener &) = delete;

	// IOPollable
	int fd() const;
	short interest() const;
	void onReadable();
	void onWritable();
	void onHangupOrError(short revents);

	const Server *getServer(void) const;
	bool hasEventLoop(void) const;

#ifdef UNIT_TEST
	unsigned short boundPort() const;
#endif
};

std::ostream &operator<<(std::ostream &out, const Listener &listener);

#endif // LISTENER_HPP
