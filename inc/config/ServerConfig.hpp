/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerConfig.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 19:25:34 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/09/04 10:00:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "config/Location.hpp"

#include <ostream>
#include <vector>

struct ListenEndpoint
{
	std::string host;	 // "" or "*" → any
	unsigned short port; // e.g. 8080
};

class ServerConfig
{
  private:
	std::vector<Location> locations;
	std::vector<ListenEndpoint> listens;
	int port;

  public:
	// Init
	ServerConfig();

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

std::ostream &operator<<(std::ostream &out, const ServerConfig &server);

#endif // CONFIG_HPP
