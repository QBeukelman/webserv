/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:14:18 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/08 12:24:54 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "EventLoop.hpp"
#include "Listener.hpp"

class WebServer
{
  private:
	// TODO: Which Listener belongs to with server?
	const ServerConfig config;
	std::vector<Listener> listeners;
	EventLoop loop;

	void initListeners();

  public:
	explicit WebServer(const ServerConfig &config);
};

#endif
