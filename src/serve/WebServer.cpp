/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   WebServer.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 12:22:05 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/09 09:29:12 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/WebServer.hpp"

WebServer::WebServer(const ServerConfig &config) : config(config)
{
	initListeners();
}

void WebServer::initListeners()
{
	// TODO: WebServer -> Handle multiple `server`s
	for (const auto &server : config.getServers())
	{
		for (const auto &endpoint : server.getListens())
		{
			listeners.push_back(Listener(endpoint.host, endpoint.port));
		}
	}
}
