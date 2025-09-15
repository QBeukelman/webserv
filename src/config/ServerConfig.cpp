/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerConfig.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 13:38:14 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/10 16:55:21 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"

#include "utility"

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
 * TODO: Add a new server to list
 */
void ServerConfig::addServer(const Server &server)
{
	this->servers.push_back(server);
}
