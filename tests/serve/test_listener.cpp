/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_listener.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:07:00 by quentinbeuk       #+#    #+#             */
/*   Updated: 2025/09/08 12:50:52 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests/doctest.h"

#include "serve/Listener.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

/*
	TODO: List

	1) Register listeners in event loop
		- Collect all the Listener objects (one per ip:port from config).
		- Add their fd() to your pollfd (or select/epoll/kqueue) set.
		- Monitor them for readable events → new connections waiting.

	2) Accept new clients: When poll() says a Listener fd is readable:
		- Call accept() (non-blocking).
		- That gives you a new client socket fd.
		- Set it non-blocking too (fcntl).
		- Add it to your poll set.

	3) Track per-client state: Create a Client or Connection struct/class:
		- Create a Client or Connection struct/class:
		- fd (client socket fd)
		- ParseContext (your HTTP request parser state)
		- Each client lives in a map: std::map<int, Client> keyed by fd.

	4) Read requests: When poll() signals a client fd as readable:
		- recv() data into a buffer.
		- Pass buffer into your RequestParser::step(ctx, data, len).
		- If a full request is parsed, hand it to your RequestHandler.

	5) Handle request

*/

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
	std::vector<unsigned short> ports;
	ports.push_back(8080);
	ports.push_back(8081);

	std::vector<Listener *> listeners;

	for (size_t i = 0; i < ports.size(); ++i)
	{
		// TODO: test_listener -> Add IP support
		REQUIRE_NOTHROW(listeners.push_back(new Listener("0.0", ports[i])));
		CHECK(listeners.back()->getFd() >= 0);

		// Verify the port is bound
		CHECK(get_bound_port(listeners.back()->getFd()) == ports[i]);
	}

	// cleanup
	for (size_t i = 0; i < listeners.size(); ++i)
	{
		delete listeners[i];
	}
}
