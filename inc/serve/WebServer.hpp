/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   WebServer.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 12:14:18 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/09 14:20:48 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

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
};

#endif
