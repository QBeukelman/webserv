/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:52:56 by hein          #+#    #+#                 */
/*   Updated: 2025/09/18 11:46:32 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/Location.hpp"

#include <ostream>
#include <vector>

enum DirectiveFlags
{
	LISTEN = 1 << 0,
	NAME = 1 << 1,
	ROOT = 1 << 2,
	INDEX = 1 << 3
};

struct ListenEndpoint
{
	std::string host;	 // "" or "*" → any
	unsigned short port; // e.g. 8080
};

struct ErrorPage
{
	unsigned short code;
	std::string route;
};

class Server
{
  private:
	unsigned int directiveFlags;
	std::vector<std::string> names;
	std::vector<ListenEndpoint> listens;
	std::vector<Location> locations;
	std::vector<std::string> indexFiles;
	std::string root;

  public:
	Server();

	// Methods
	Location findLocation(const std::string &requestPath) const;

	// Getters / Setters
	std::string &getName(void) const;
	std::vector<Location> getLocations(void) const;
	void setLocation(const Location &location);

	std::vector<ListenEndpoint> getListens(void) const;
	std::vector<std::string> getNames(void) const;
	std::string getRoot(void) const;
	std::vector<std::string> getIndexFiles(void) const;

	// Set Parsed Data
	bool setListen(const ListenEndpoint &listen);
	bool setName(const std::string &name);
	void setRoot(const std::string &root);
	bool setIndex(const std::string &index);

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
