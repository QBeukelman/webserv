/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:57:51 by hein          #+#    #+#                 */
/*   Updated: 2025/09/12 10:06:09 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"

Server::Server() : name("Server")
{
}

Server::Server(std::string name) : name(name)
{
}

// GETTER / SETTER
// ____________________________________________________________________________
std::string Server::getName(void) const
{
	return (this->name);
}

void Server::setName(std::string newName)
{
	this->name = newName;
}

std::vector<Location> Server::getLocations(void) const
{
	return (this->locations);
}

void Server::setLocations(const std::vector<Location> &newLocations)
{
	this->locations = newLocations;
}

void Server::addLocation(const Location &location)
{
	this->locations.push_back(location);
}

std::vector<ListenEndpoint> Server::getListens(void) const
{
	return (this->listens);
}

void Server::setListens(std::vector<ListenEndpoint> newListens)
{
	this->listens = newListens;
}

// GETTER / SETTER
// ____________________________________________________________________________
const char *Server::LocationNotFoundException::what() const throw()
{
	return ("Exception: Server `findLocation()` not found");
}
