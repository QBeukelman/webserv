/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   WebServer.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 12:22:05 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/13 13:56:35 by quentinbeuk   ########   odam.nl         */
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
	size_t total = 0;
	for (const auto &s : config.getServers())
		total += s.getListens().size();
	listeners.reserve(total);

	for (const auto &s : config.getServers())
	{
		for (const auto &ep : s.getListens())
		{
			auto ptr = std::make_unique<Listener>(ep.host, ep.port, &s, &loop);
			Listener *raw = ptr.get();
			loop.add(raw);
			fdToServer[raw->fd()] = &s;
			listeners.push_back(std::move(ptr));
		}
	}
}

void WebServer::run()
{
	auto &shut = ShutdownPollable::instance();
	shut.setOnBeginDrain([&]() {
		loop.stop();
		std::exit(1);
	});
	if (!shut.initialize())
	{
		std::exit(1);
	}
	loop.add(&ShutdownPollable::instance());
	loop.run();
}

const std::vector<std::unique_ptr<Listener>> &WebServer::getListeners() const
{
	return listeners;
}

// OVERLOAD
// ____________________________________________________________________________
static void printListeners(const std::vector<std::unique_ptr<Listener>> &listeners)
{
	for (const auto &lp : listeners)
	{
		if (lp)
			std::cout << *lp << '\n';
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
	return (listeners.front()->boundPort());
}
#endif
