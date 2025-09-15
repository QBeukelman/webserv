/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:52:56 by hein          #+#    #+#                 */
/*   Updated: 2025/09/15 20:00:24 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "config/Location.hpp"

#include <ostream>
#include <vector>

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
	std::vector<std::string> names;
	std::vector<ListenEndpoint> listens;
	std::vector<Location> locations;

  public:
	Server();

	// Methods
	Location findLocation(const std::string &requestPath) const;

	// Getters / Setters
	std::vector<Location> getLocations(void) const;
	void setLocation(const Location &location);

	std::vector<ListenEndpoint> getListens(void) const;

	bool setListen(const ListenEndpoint &listen);

	bool setName(const std::string &name);
	std::string &getName(void) const;

	void printNames();
	void printListens();

	class LocationNotFoundException : public std::exception
	{
	  public:
		virtual const char *what() const throw();
	};
};

#endif
