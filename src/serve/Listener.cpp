/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Listener.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/04 09:21:09 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/03 10:10:58 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/Listener.hpp"
#include "log/Logger.hpp"

// CONSTRUCTORS
// ____________________________________________________________________________
/*
 * Construct a Listener from `ip` and `port`
 *
 * Warning:
 * 	- Throws
 */
Listener::Listener(const std::string &ip, unsigned short port, const Server *server, EventLoop *loop)
	: fd_(-1), server(server), loop(loop)
{

	if (port != 0 && port < 1024)
	{
		Logger::error("Listener::Listener: Will not bind port `< 1024`");
		throw std::runtime_error("Will not bind port `< 1024`");
	}

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
	setNonBlocking(fd_);

	// 4) Bind & Listen
	bindAndListen(ip, port);
}

Listener::Listener(Listener &&other) noexcept : fd_(other.fd_)
{
	other.fd_ = -1;
}

Listener &Listener::operator=(Listener &&other) noexcept
{
	if (this != &other)
	{
		closeIfValid();
		fd_ = other.fd_;
		other.fd_ = -1;
	}
	return (*this);
}

Listener::~Listener()
{
	closeIfValid();
}

int Listener::fd() const
{
	return (this->fd_);
}

short Listener::interest() const
{
	return (POLLIN);
}

// LOGGING
// ____________________________________________________________________________
static void logListnerCreated(const std::string &ip, unsigned int port)
{
	std::ostringstream oss;
	std::cout << LIGHT_GRAY << "====================================================\n";
	oss << "Listner created → ";
	oss << UNDERLINE << "http://" << ip << ":" << port << "/" << RESET_STYLE;
	Logger::success(oss.str());
	std::cout << LIGHT_GRAY << "====================================================\n";
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
void Listener::setNonBlocking(int fd)
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
void Listener::bindAndListen(const std::string &ip, unsigned short port)
{
	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0, all interfaces
	addr.sin_port = htons(port);			  // Convert to network byte order

	logListnerCreated(ip, port);

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

unsigned short Listener::getLocalPort(int fd) const
{
	sockaddr_storage ss;
	socklen_t s_len = sizeof(ss);

	if (::getsockname(fd, reinterpret_cast<sockaddr *>(&ss), &s_len) == 0)
	{
		if (ss.ss_family == AF_INET)
			return (ntohs(reinterpret_cast<sockaddr_in *>(&ss)->sin_port));
		else if (ss.ss_family == AF_INET6)
			return (ntohs(reinterpret_cast<sockaddr_in6 *>(&ss)->sin6_port));
	}
	return (0);
}

void Listener::closeIfValid()
{
	if (fd_ >= 0)
		::close(fd_);
	fd_ = -1;
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
		Logger::info("Listener::onReadable() → Connection accepted: " + std::to_string(cfd));

		// TODO: Set non blocking for (fd).
		setNonBlocking(cfd);

		// Create a Connection for this client and register it.
		unsigned short localPort = getLocalPort(cfd);
		Connection *connection = new Connection(cfd, server, loop, localPort);
		loop->add(connection);
	}
}

/*
 * Emply function: A Listner will not write.
 */
void Listener::onWritable()
{
}

void Listener::onHangupOrError(short revents)
{
	Logger::error("Listener::onHangupOrError → " + std::to_string(revents));
	loop->closeLater(this);
}

// GETTERS & SETTERS
// ____________________________________________________________________________
const Server *Listener::getServer(void) const
{
	return (this->server);
}

const bool Listener::hasEventLoop(void) const
{
	if (this->server)
		return (true);
	return (false);
}

// OVERLOAD
// ____________________________________________________________________________
std::ostream &operator<<(std::ostream &out, const Listener &listener)
{
	const std::string hasEventLoop = listener.hasEventLoop() ? "true" : "false";

	out << "Listener FD: " << listener.fd() << "\nListener Server: " << listener.getServer()->getName(0)
		<< "\nListener has EventLoop: " << hasEventLoop << std::endl;

	return (out);
}

// TESTING
// ____________________________________________________________________________
#define UNIT_TEST
#ifdef UNIT_TEST

#include <netinet/in.h>
#include <sys/socket.h>

// TODO: Listener -> Use getLocalPort() with getsockname()
unsigned short Listener::boundPort() const
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	if (::getsockname(fd(), reinterpret_cast<sockaddr *>(&addr), &len) != 0)
		throw std::runtime_error("getsockname failed");
	return (ntohs(addr.sin_port));
}
#endif
