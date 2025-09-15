/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:57:51 by hein          #+#    #+#                 */
/*   Updated: 2025/09/15 20:25:54 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"

#include <algorithm>

Server::Server()
{
}

// GETTER / SETTER
// ____________________________________________________________________________
// std::string Server::getName(void) const
// {
// 	std::string name = names.begin();
// 	return (names.begin());
// }

std::vector<Location> Server::getLocations(void) const
{
	return (this->locations);
}

void Server::setLocation(const Location &location)
{
	this->locations.push_back(location);
}

std::vector<ListenEndpoint> Server::getListens(void) const
{
	return (this->listens);
}

static bool listenConflict(const ListenEndpoint &a, const ListenEndpoint &b)
{
	if (a.port != b.port)
	{
		return (false);
	}
	if (a.host == "0.0.0.0" || b.host == "0.0.0.0")
	{
		return (true);
	}
	return (a.host == b.host);
}
bool Server::setListen(const ListenEndpoint &listen)
{
	for (ListenEndpoint &current : listens)
	{
		if (listenConflict(current, listen))
		{
			return (false);
		}
	}
	this->listens.push_back(listen);
	return (true);
}

bool Server::setName(const std::string &name)
{
	if (std::find(names.begin(), names.end(), name) != names.end())
	{
		return (false);
	}
	this->names.push_back(name);
	return (true);
}

void Server::printNames()
{
	for (std::string s : names)
	{
		std::cout << s << std::endl;
	}
}

void Server::printListens()
{
	for (ListenEndpoint i : listens)
	{
		std::cout << "Host [ " + i.host + " ] Port [ " + std::to_string(i.port) + " ]" << std::endl;
	}
}

const char *Server::LocationNotFoundException::what() const throw()
{
	return ("Exception: Server `findLocation()` not found");
}