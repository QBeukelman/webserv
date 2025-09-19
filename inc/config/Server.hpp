/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:52:56 by hein          #+#    #+#                 */
/*   Updated: 2025/09/19 08:46:46 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/Location.hpp"
#include "config/models/DirectiveFlags.hpp"
#include "config/models/ErrorPage.hpp"
#include "config/models/ListenEndpoint.hpp"

#include <ostream>
#include <string>
#include <vector>

enum DirectiveFlags
{
	LISTEN = 1 << 0,
	NAME = 1 << 1,
	ROOT = 1 << 2,
	INDEX = 1 << 3,
	ERRORPAGE = 1 << 4,
	MAXBODYSIZE = 1 << 5
};

struct ListenEndpoint
{
	std::string host;	 // "" or "*" → any
	unsigned short port; // e.g. 8080
};

struct ErrorPage
{
	unsigned short code;
	std::string path;
};

class Server
{
  private:
	unsigned int directiveFlags;
	std::string name;
	std::vector<ListenEndpoint> listens;
	std::vector<Location> locations;
	std::vector<std::string> indexFiles;
	std::string root;
	std::vector<ErrorPage> errorPages;
	size_t maxBodySize;

	bool listenConflict(const ListenEndpoint &a, const ListenEndpoint &b);

  public:
	Server();
	Server(std::string name);

	// Methods
	Location findLocation(const std::string &requestPath) const;

	// -- Getters & Setters --
	// Name
	const std::string getName(void) const;
	void setName(std::string newName);

	// Listens
	std::vector<ListenEndpoint> getListens(void) const;
	std::vector<std::string> getNames(void) const;
	std::string getRoot(void) const;
	std::vector<std::string> getIndexFiles(void) const;
	std::vector<ErrorPage> getErrorPages(void) const;
	std::size_t getMaxBodySize(void) const;
	bool setListen(const ListenEndpoint &listen);
	void setListens(std::vector<ListenEndpoint> &);

	// Location
	std::vector<Location> getLocations(void) const;
	void addLocation(const Location &location);
	void setLocations(const std::vector<Location> &);

	// Set Parsed Data
	void setRoot(const std::string &root);
	bool setIndex(const std::string &index);
	bool setErrorPage(const ErrorPage &errorPage);
	void setMaxBodySize(const std::size_t &maxBody);

	// Bitmask Methods
	void markDirective(unsigned int directive);
	bool hasDirective(unsigned int directive);
	bool requiredDirectives(unsigned int required);

	class LocationNotFoundException : public std::exception
	{
	  public:
		virtual const char *what() const throw();
	};
};

std::ostream &operator<<(std::ostream &os, Server &server);

#endif
