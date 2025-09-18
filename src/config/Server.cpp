/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:57:51 by hein          #+#    #+#                 */
/*   Updated: 2025/09/18 23:49:24 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"

#include <algorithm>
#include <iostream>

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

const std::string &Server::getName(void) const
{
	return (this->names.front());
}

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
std::vector<std::string> Server::getNames(void) const
{
	return (this->names);
}
std::string Server::getRoot(void) const
{
	return (this->root);
}

std::vector<std::string> Server::getIndexFiles(void) const
{
	return (this->indexFiles);
}

std::vector<ErrorPage> Server::getErrorPages(void) const
{
	return (this->errorPages);
}

std::size_t Server::getMaxBodySize(void) const
{
	return (this->maxBodySize);
}

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

bool Server::setIndex(const std::string &index)
{
	if (std::find(indexFiles.begin(), indexFiles.end(), index) != indexFiles.end())
	{
		return (false);
	}
	indexFiles.push_back(index);
	return (true);
}

bool Server::setErrorPage(const ErrorPage &errorPage)
{
	for (auto it : errorPages)
	{
		if (it.code == errorPage.code)
		{
			return (false);
		}
	}
	errorPages.push_back(errorPage);
	return (true);
}

void Server::setMaxBodySize(const std::size_t &maxBody)
{
	maxBodySize = maxBody;
}

// BITMASK FLAGG METHODS
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

// PRINT SERVER
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
		os << errorPage.code << "-" << errorPage.path << " ";
	}

	os << "\nMax Body Size " << server.getMaxBodySize();

	return (os);
}