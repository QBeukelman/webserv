/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:52:56 by hein          #+#    #+#                 */
/*   Updated: 2025/10/01 15:21:01 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/Location.hpp"
#include "config/config_parser/IConfigBlock.hpp"
#include "config/models/DirectiveFlags.hpp"
#include "config/models/ErrorPage.hpp"
#include "config/models/ListenEndpoint.hpp"
#include "http/models/HttpRequestLimits.hpp"

#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

class Server : public IConfigBlock
{
  private:
	unsigned int directiveFlags;
	std::vector<std::string> names;
	std::vector<ListenEndpoint> listens;
	std::vector<Location> locations;
	std::vector<std::string> index_files;
	std::string root;
	std::vector<ErrorPage> errorPages;
	HttpRequestLimits limits;

	bool listenConflict(const ListenEndpoint &a, const ListenEndpoint &b);

  public:
	Server();
	Server(std::string name);

	// Methods
	Location findLocation(const std::string &rawPath) const;

	// -- Getters & Setters --
	// Names
	const std::string &getName(const int index) const;
	std::vector<std::string> getNames(void) const;
	void setName(std::string newName);

	// Listens
	std::vector<ListenEndpoint> getListens(void) const;
	bool setListen(const ListenEndpoint &listen);
	void setListens(std::vector<ListenEndpoint> &);

	// Location
	std::vector<Location> getLocations(void) const;
	void addLocation(const Location &location);
	void setLocations(const std::vector<Location> &);

	// Index files
	std::vector<std::string> getIndexFiles(void) const;

	// Error pages
	std::vector<ErrorPage> getErrorPages(void) const;
	bool addErrorPage(const ErrorPage &errorPage);

	// Limits
	HttpRequestLimits getLimits(void) const;
	void setMaxBodySize(const std::size_t &maxBody);

	// IConfigBlock overrides
	void setRoot(const std::string &root); /* override */
	std::string getRoot(void) const;	   /* override */

	bool addIndexFile(const std::string &index); /* override */

	void markDirective(unsigned int directive);		  /* override */
	bool hasDirective(unsigned int directive);		  /* override */
	bool requiredDirectives(unsigned int directives); /* override */

	class LocationNotFoundException : public std::exception
	{
	  public:
		virtual const char *what() const throw();
	};
};

std::ostream &operator<<(std::ostream &os, Server &server);

#endif
