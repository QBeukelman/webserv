/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 14:57:51 by hein              #+#    #+#             */
/*   Updated: 2025/09/17 09:03:59 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"

#include <algorithm>

Server::Server()
{
	directiveFlags = 0;
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

void Server::setListens(std::vector<ListenEndpoint> &newListens)
{
	this->listens = newListens;
// SET PARSED DATA
// ____________________________________________________________________________

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

void Server::setRoot(const std::string &root)
{
	this->root = root;
}

// BITMASK FLAGG FUNCTIONS
// ____________________________________________________________________________

void Server::markDirective(unsigned int directive)
{
	directiveFlags |= directive;
}

bool Server::hasDirective(unsigned int directive)
{
	return ((directiveFlags & directive) != 0);
}

bool Server::requiredDirectives(unsigned int required)
{
	return ((directiveFlags & required) == required);
}

// EXCEPTIONS
// ____________________________________________________________________________

const char *Server::LocationNotFoundException::what() const throw()
{
	return ("Exception: Server `findLocation()` not found");
}