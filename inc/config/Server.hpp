/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 14:52:56 by hein          #+#    #+#                 */
/*   Updated: 2025/09/08 14:56:56 by hein          ########   odam.nl         */
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

class Server
{
	private:
		std::vector<Location> locations;
	std::vector<ListenEndpoint> listens;
	int port;
	
	public:
		Server();

		// Methods
		const Location *findLocation(std::string requestPath) const;

		// Getters / Setters
		std::vector<Location> getLocations(void) const;
		void setLocations(std::vector<Location>);

		std::vector<ListenEndpoint> getListens(void) const;
		void setListens(std::vector<ListenEndpoint>);

		int getPort(void) const;
		void setPort(int);
};

#endif
