/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:57:51 by hein          #+#    #+#                 */
/*   Updated: 2025/09/08 14:58:45 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "config/Server.hpp"

Server::Server() {}

// GETTER / SETTER
// ____________________________________________________________________________
std::vector<Location> Server::getLocations(void) const
{
	return (this->locations);
}

void Server::setLocations(std::vector<Location> newLocations)
{
	this->locations = newLocations;
}

std::vector<ListenEndpoint> Server::getListens(void) const
{
	return (this->listens);
}

void Server::setListens(std::vector<ListenEndpoint> newListens)
{
	this->listens = newListens;
}

int Server::getPort(void) const
{
	return (this->port);
}

void Server::setPort(int newPort)
{
	this->port = newPort;
}
