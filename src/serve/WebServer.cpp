/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:22:05 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/15 12:00:30 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serve/WebServer.hpp"

// CONSTRUCTORS
// ____________________________________________________________________________
WebServer::WebServer(const ServerConfig &config) : config(config)
{
	initListeners();
}

void WebServer::initListeners()
{
	for (const auto &server : config.getServers())
	{
		for (const auto &endpoint : server.getListens())
		{
			listeners.emplace_back(endpoint.host, endpoint.port, &server, &loop);
			loop.add(&listeners.back());
			fdToServer[listeners.back().fd()] = &server;
		}
	}
}

void WebServer::run()
{
	loop.run();
}

const std::vector<Listener> &WebServer::getListeners(void) const
{
	return (this->listeners);
}

// OVERLOAD
// ____________________________________________________________________________
static void printListeners(const std::vector<Listener> &listeners)
{
	for (const Listener &l : listeners)
	{
		std::cout << l << std::endl;
	}
}

std::ostream &operator<<(std::ostream &out, const WebServer &webServer)
{
	out << "\n=== WEB SERVER ===\n"
		<< "--- Listeners ---\n"
		<< "Count: " << webServer.getListeners().size() << std::endl;
	printListeners(webServer.getListeners());
	return (out);
}

// TESTING
// ____________________________________________________________________________
#define UNIT_TEST
#ifdef UNIT_TEST
unsigned short WebServer::primaryPort() const
{
	if (listeners.empty())
		throw std::runtime_error("no listeners");
	return (listeners.front().boundPort());
}
#endif
