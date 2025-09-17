/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:14:18 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/15 10:19:03 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP
#define UNIT_TEST

#include "EventLoop.hpp"
#include "Listener.hpp"

#include <map>

class WebServer
{
  private:
	const ServerConfig config;
	std::vector<Listener> listeners;
	std::map<int, const Server *> fdToServer;
	EventLoop loop;

	void initListeners();

  public:
	explicit WebServer(const ServerConfig &config);
	void run();

	// Getters & Setters
	const std::vector<Listener> &getListeners(void) const;

#ifdef UNIT_TEST
	unsigned short primaryPort() const;
#endif
};

std::ostream &operator<<(std::ostream &out, const WebServer &webServer);

#endif
