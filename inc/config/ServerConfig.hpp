/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerConfig.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 19:25:34 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/10/02 10:18:10 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "config/Location.hpp"
#include "config/Server.hpp"

#include <iostream>
#include <ostream>
#include <vector>

/*
 * ServerConfig holds vector of Server
 */
class ServerConfig
{
  private:
	std::vector<Server> servers;
	int bracketDepth = 0;

  public:
	// Init
	ServerConfig();

	void addServer(const Server &server);
	const std::vector<Server> &getServers(void) const;

	void incrementDepth(void);
	void decrementDepth(void);
	bool depthProperlyClosed(void);
};

#endif // CONFIG_HPP
