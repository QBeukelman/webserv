/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerConfig.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 19:25:34 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/09/09 09:30:10 by quentinbeuk   ########   odam.nl         */
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

  public:
	// Init
	ServerConfig();

	void addServer(const Server &server);
	std::vector<Server> getServers(void) const;
};

// TODO: Add overload for ServerConfig
// std::ostream &operator<<(std::ostream &out, const ServerConfig &server);

#endif // CONFIG_HPP
