/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:22:05 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/08 12:48:00 by qbeukelm         ###   ########.fr       */
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
	// for (s : config.servers) {  }

	for (auto &endpoint : config.getListens())
	{
		listeners.push_back(Listener(endpoint.host, endpoint.port));
	}
}
