/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerConfig.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 13:38:14 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/03 10:08:14 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "log/Logger.hpp"

// CONSTRUCTORS
// ____________________________________________________________________________
ServerConfig::ServerConfig() : servers(std::vector<Server>())
{
}

const std::vector<Server> &ServerConfig::getServers(void) const
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
	Logger::info("Adding server to config: " + server.getName(0));

	servers.push_back(server);
}

void ServerConfig::incrementDepth(void)
{
	++bracketDepth;
}
void ServerConfig::decrementDepth(void)
{
	--bracketDepth;
}
bool ServerConfig::depthProperlyClosed(void)
{
	return (bracketDepth == 0);
}
