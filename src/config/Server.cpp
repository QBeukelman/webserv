/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:57:51 by hein          #+#    #+#                 */
/*   Updated: 2025/10/02 20:23:48 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"

#include <algorithm>
#include <iostream>

Server::Server() : directiveFlags(0)
{
	this->setName("Server");
}

Server::Server(std::string name) : directiveFlags(0)
{
	this->setName(name);
}

// GETTER / SETTER
// ____________________________________________________________________________
// ===== Names =====
const std::string &Server::getName(const int index) const
{
	return (this->names[index]);
}

std::vector<std::string> Server::getNames(void) const
{
	return (this->names);
}

void Server::setName(std::string newName)
{
	this->names.push_back(newName);
}

// ===== Listens =====
std::vector<ListenEndpoint> Server::getListens(void) const
{
	return (this->listens);
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

void Server::setListens(std::vector<ListenEndpoint> &newListens)
{
	this->listens = newListens;
}

// ===== Locations =====
std::vector<Location> Server::getLocations(void) const
{
	return (this->locations);
}

bool Server::addLocation(const Location &location)
{
	for (auto it = locations.cbegin(); it != locations.cend(); ++it)
	{
		if (it->getPrefix() == location.getPrefix())
		{
			return (false);
		}
	}
	this->locations.push_back(location);
	return (true);
}

void Server::setLocations(const std::vector<Location> &newLocations)
{
	this->locations = newLocations;
}

// ===== Root =====
std::string Server::getRoot(void) const
{
	return (this->root);
}

void Server::setRoot(const std::string &root)
{
	this->root = root;
}

// ===== Index Files =====
std::vector<std::string> Server::getIndexFiles(void) const
{
	return (this->index_files);
}

bool Server::addIndexFile(const std::string &index)
{
	if (std::find(index_files.begin(), index_files.end(), index) != index_files.end())
	{
		return (false);
	}
	index_files.push_back(index);
	return (true);
}

// ===== Error Pages =====
std::vector<ErrorPage> Server::getErrorPages(void) const
{
	return (this->errorPages);
}

bool Server::addErrorPage(const ErrorPage &errorPage)
{
	for (auto it : errorPages)
	{
		if (it.httpStatus == errorPage.httpStatus)
		{
			return (false);
		}
	}
	errorPages.push_back(errorPage);
	return (true);
}

// ===== Limits =====
HttpRequestLimits Server::getLimits(void) const
{
	return (this->limits);
}

void Server::setMaxBodySize(const std::size_t &maxBody)
{
	this->limits.max_body_size = maxBody;
}

// BITMASK FLAG METHODS
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

// PRIVATE
// ____________________________________________________________________________
bool Server::listenConflict(const ListenEndpoint &a, const ListenEndpoint &b)
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

// EXCEPTIONS
// ____________________________________________________________________________
const char *Server::LocationNotFoundException::what() const throw()
{
	return ("Exception: Server `findLocation()` not found");
}

// OVERLOAD
// ____________________________________________________________________________
std::ostream &operator<<(std::ostream &os, Server &server)
{
	os << "Server Names: ";
	for (const auto &name : server.getNames())
	{
		os << name << " ";
	}

	os << "\nListens: ";
	for (const auto &listen : server.getListens())
	{
		os << listen.host << ":" << listen.port << " ";
	}

	os << "\nRoot: " << server.getRoot();

	os << "\nIndexFiles; ";
	for (const auto &index : server.getIndexFiles())
	{
		os << index << " ";
	}

	os << "\nErrorPages: ";
	for (const auto &errorPage : server.getErrorPages())
	{
		os << errorPage.httpStatus << "-" << errorPage.path << " ";
	}

	os << "\nMax Body Size " << server.getLimits().max_body_size;

	return (os);
}
