/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:52:56 by hein          #+#    #+#                 */
/*   Updated: 2025/09/15 18:38:12 by hein          ########   odam.nl         */
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
	const Location *findLocation(std::string requestPath) const;

	// Getters / Setters
	std::vector<Location> getLocations(void) const;
	void setLocation(const Location &location);

	std::vector<ListenEndpoint> getListens(void) const;
	bool setListen(const ListenEndpoint &listen);

	bool setName(const std::string &name);

	void printNames();
	void printListens();
};

#endif
