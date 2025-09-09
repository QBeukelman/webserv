/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_listener.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/04 17:07:00 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/09 17:24:11 by quentinbeuk   ########   odam.nl         */
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
	std::vector<unsigned short> ports;
	ports.push_back(8080);
	ports.push_back(8081);

	std::vector<Listener *> listeners;

	for (size_t i = 0; i < ports.size(); ++i)
	{
		// TODO: test_listener -> Add IP support
		REQUIRE_NOTHROW(listeners.push_back(new Listener("0.0", ports[i])));
		CHECK(listeners.back()->fd() >= 0);

		// Verify the port is bound
		CHECK(get_bound_port(listeners.back()->fd()) == ports[i]);
	}

	// cleanup
	for (size_t i = 0; i < listeners.size(); ++i)
	{
		delete listeners[i];
	}
}
