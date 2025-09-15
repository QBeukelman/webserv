/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 14:52:56 by hein              #+#    #+#             */
/*   Updated: 2025/09/15 09:43:37 by qbeukelm         ###   ########.fr       */
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

	ListenEndpoint() : host(""), port(8080)
	{
	}
	ListenEndpoint(const std::string &h, unsigned short p) : host(h), port(p)
	{
	}
};

class Server
{
  private:
	std::string name;
	std::vector<Location> locations;
	std::vector<ListenEndpoint> listens;

  public:
	Server();
	Server(std::string name);

	// Methods
	Location findLocation(const std::string &requestPath) const;

	// Getters / Setters
	std::string getName(void) const;
	void setName(std::string newName);

	std::vector<Location> getLocations(void) const;
	void setLocations(const std::vector<Location> &);
	void addLocation(const Location &location);

	std::vector<ListenEndpoint> getListens(void) const;
	void setListens(std::vector<ListenEndpoint> &);

	class LocationNotFoundException : public std::exception
	{
	  public:
		virtual const char *what() const throw();
	};
};

#endif
