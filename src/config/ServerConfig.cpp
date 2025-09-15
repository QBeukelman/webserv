/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerConfig.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 13:38:14 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/09 11:36:50 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "log/Logger.hpp"

// CONSTRUCTORS
// ____________________________________________________________________________
ServerConfig::ServerConfig() : servers(std::vector<Server>())
{
}

std::vector<Server> ServerConfig::getServers(void) const
{
	return (this->servers);
}

// MEMBERS
// ____________________________________________________________________________
/*
 * Add a new server to `vector<Server>`
 */
void ServerConfig::addServer(const Server &server)
{
	Logger::debug(Logger::join("Adding server to server config: ", server.getName()));

	servers.push_back(server);
}
