/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Listener.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/04 09:21:09 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/04 17:22:18 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/Listener.hpp"
#include "log/Logger.hpp"

#include <cstring>

// CONSTRUCTORS
// ____________________________________________________________________________
Listener::Listener(unsigned short port) : fd(-1)
{

	// 1) Create a TCP/IPv4 socket
	fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		Logger::error("Listener::Listener: socket() failed");
		throw std::runtime_error("socket() failed");
	}

	// 2) Allow quick rebinding after restart
	setReuseAddress();

	// 3) Set the scoket to non-blocking mode
	setNonBlocking();

	// 4) Bind & Listen
	bindAndListen(port);
}

Listener::~Listener()
{
	if (fd >= 0)
	{
		::close(fd);
		fd = -1;
	}
}

int Listener::getFd() const
{
	return (this->fd);
}

// MEMBERS
// ____________________________________________________________________________
/*
 * Restart server quickly on the same port without waiting
 * for TCP “TIME_WAIT” sockets to expire. Otherwise you might see “Address already in use”.
 */
void Listener::setReuseAddress()
{
	int opt = 1;

	if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		::close(fd);
		Logger::error("Listener::Listener: setsockopt(SO_REUSEADDR) failed");
		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
	}
}

/*
 * Makes the socket non-blocking.
 * This means calls like accept() and recv() won’t freeze the program if nothing is ready
 */
void Listener::setNonBlocking()
{
	if (::fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
	{
		::close(fd);
		Logger::error("Listener::Listener: fcntl(O_NONBLOCK) failed");
		throw std::runtime_error("fcntl(O_NONBLOCK) failed");
	}
}

/*
 * `bind()` associates the socket with the chosen port (and all local IPs).
 * `listen()` turns it into a listening socket so clients can connect. SOMAXCONN
 */
void Listener::bindAndListen(unsigned short port)
{
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0, all interfaces
	addr.sin_port = htons(port);			  // convert to network byte order

	if (::bind(fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
	{
		::close(fd);
		Logger::error("Listener::Listener: bind() failed");
		throw std::runtime_error("bind() failed");
	}

	if (::listen(fd, SOMAXCONN) < 0)
	{
		::close(fd);
		Logger::error("Listener::Listener: listen() failed");
		throw std::runtime_error("listen() failed");
	}
}
