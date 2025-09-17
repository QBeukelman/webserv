/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 14:52:56 by hein              #+#    #+#             */
/*   Updated: 2025/09/17 09:54:50 by qbeukelm         ###   ########.fr       */
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

class Server
{
  private:
	unsigned int directiveFlags;
	std::string name;
	std::vector<ListenEndpoint> listens;
	std::vector<Location> locations;
	std::string root;

	bool listenConflict(const ListenEndpoint &a, const ListenEndpoint &b);

  public:
	Server();
	Server(std::string name);

	// Methods
	Location findLocation(const std::string &requestPath) const;

	// -- Getters & Setters --
	// Name
	std::string getName(void) const;
	void setName(std::string newName);

	// Listens
	std::vector<ListenEndpoint> getListens(void) const;
	bool setListen(const ListenEndpoint &listen);
	void setListens(std::vector<ListenEndpoint> &);

	// Location
	std::vector<Location> getLocations(void) const;
	void addLocation(const Location &location);
	void setLocations(const std::vector<Location> &);

	// Set Parsed Data
	void setRoot(const std::string &root);

	// Bitmask Methods
	void markDirective(unsigned int directive);
	bool hasDirective(unsigned int directive);
	bool requiredDirectives(unsigned int directives);

	class LocationNotFoundException : public std::exception
	{
	  public:
		virtual const char *what() const throw();
	};
};

#endif
