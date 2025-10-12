/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_listener.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/04 17:07:00 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/10 09:56:27 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tests/doctest.h"

#include "serve/Listener.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

// Check that a socket is bound to the expected port
static unsigned short get_bound_port(int fd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	if (::getsockname(fd, reinterpret_cast<struct sockaddr *>(&addr), &len) < 0)
	{
		throw std::runtime_error("getsockname() failed");
	}
	return ntohs(addr.sin_port);
}

TEST_CASE("Listener::Listener: bind and listen on ports")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	std::vector<unsigned short> ports;
	ports.push_back(8080);
	ports.push_back(8081);

	std::vector<Listener *> listeners;
	const Server server;
	EventLoop loop;

	for (size_t i = 0; i < ports.size(); ++i)
	{
		const std::string ip = "127.0.0.0";

		Listener *l = NULL;
		REQUIRE_NOTHROW(l = new Listener(ip, ports[i], &server, &loop));
		listeners.push_back(l);

		CHECK(l->fd() >= 0);
		CHECK(get_bound_port(l->fd()) == ports[i]);
	}

	// cleanup
	for (size_t i = 0; i < listeners.size(); ++i)
	{
		delete listeners[i];
	}
}
