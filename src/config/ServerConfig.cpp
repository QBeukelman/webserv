/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerConfig.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 13:38:14 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/03 14:42:04 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"

// CONSTRUCTORS
// ____________________________________________________________________________
ServerConfig::ServerConfig() : locations(std::vector<Location>()), port(0)
{
}

// GETTER / SETTER
// ____________________________________________________________________________
std::vector<Location> ServerConfig::getLocations(void) const
{
	return (this->locations);
}

void ServerConfig::setLocations(std::vector<Location> newLocations)
{
	this->locations = newLocations;
}

int ServerConfig::getPort(void) const
{
	return (this->port);
}

void ServerConfig::setPort(int newPort)
{
	this->port = newPort;
}

// OVERLOAD
// ____________________________________________________________________________
std::ostream &operator<<(std::ostream &out, const ServerConfig &server)
{
	out << "\n=== ServerConfig ===\n"
		<< "\nPort: " << server.getPort() << "\n";

	std::vector<Location> locations = server.getLocations();
	for (const auto &l : locations)
	{
		out << l;
	}

	return (out);
}
