/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Listener.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/04 09:21:09 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/12 10:32:41 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/Listener.hpp"
#include "log/Logger.hpp"

// CONSTRUCTORS
// ____________________________________________________________________________
Listener::Listener(const std::string &ip, unsigned short port, const Server *server, EventLoop *loop)
	: fd_(-1), server(server), loop(loop)
{

	// 1) Create a TCP/IPv4 socket
	fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ < 0)
	{
		Logger::error("Listener::Listener: socket() failed");
		throw std::runtime_error("socket() failed");
	}

	// 2) Allow quick rebinding after restart
	setReuseAddress();

	// 3) Set the scoket to non-blocking mode
	setNonBlocking();

	// 4) Bind & Listen
	bindAndListen(ip, port);
}

Listener::~Listener()
{
	if (fd_ >= 0)
	{
		::close(fd_);
		fd_ = -1;
	}
}

int Listener::fd() const
{
	return (this->fd_);
}

short Listener::interest() const
{
	return (POLLIN);
}

// PRIVATE
// ____________________________________________________________________________
/*
 * Restart server quickly on the same port without waiting
 * for TCP “TIME_WAIT” sockets to expire. Otherwise you might see “Address already in use”.
 */
void Listener::setReuseAddress()
{
	int opt = 1;

	if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		::close(fd_);
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
	if (::fcntl(fd_, F_SETFL, O_NONBLOCK) < 0)
	{
		::close(fd_);
		Logger::error("Listener::Listener: fcntl(O_NONBLOCK) failed");
		throw std::runtime_error("fcntl(O_NONBLOCK) failed");
	}
}

/*
 * `bind()` associates the socket with the chosen port (and all local IPs).
 * `listen()` turns it into a listening socket so clients can connect. SOMAXCONN
 */
void Listener::bindAndListen(const std::string &ip, unsigned short port)
{
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0, all interfaces
	addr.sin_port = htons(port);			  // convert to network byte order

	// TODO: How to use ip?
	std::ostringstream oss;
	oss << "Listener created for IP: " << ip << " PORT: " << port;
	Logger::info(oss.str());

	if (::bind(fd_, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
	{
		::close(fd_);
		Logger::error("Listener::Listener: bind() failed");
		throw std::runtime_error("bind() failed");
	}

	if (::listen(fd_, SOMAXCONN) < 0)
	{
		::close(fd_);
		Logger::error("Listener::Listener: listen() failed");
		throw std::runtime_error("listen() failed");
	}
}

// PUBLIC
// ____________________________________________________________________________
/*
 * A listening socket is marked as readable by `poll()` when
 * a new client is waiting to connect.
 *
 * `onReadable()` calls `accept()`, get a new client file descriptor
 * and wrap it in a Connection obj.
 *
 * Notes:
 * Readable = accept a new connection.
 */
void Listener::onReadable()
{
	// TODO: Listener::onReadable().
	while (true)
	{
		sockaddr_in address;
		socklen_t adrs_len = sizeof(address);

		int cfd = ::accept(fd_, reinterpret_cast<sockaddr *>(&address), &adrs_len);

		if (cfd < 0)
		{
			// Non-blocking accept: nothing more to accept now.
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;

			Logger::error("Listener::onReadable() failed → " + std::string(std::strerror(errno)));
			break;
		}

		// TODO: Set non blocking for (fd).
		setNonBlocking();

		// Create a Connection for this client and register it.
		Connection *connection = new Connection(cfd, server, loop);
		loop->add(connection);
	}
}

// A Listner will not write
void Listener::onWritable()
{
}

void Listener::onHangupOrError(short revents)
{
	// TODO: Listener::onHangupOrError().
	Logger::error("Listener::onHangupOrError → " + std::to_string(revents));
}
