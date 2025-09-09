/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_findLocation.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 14:05:59 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/09 11:08:42 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tests/doctest.h"

#include "config/ServerConfig.hpp"

TEST_CASE("ServerConfig: findLocation() basic matching")
{
	// Given
	ServerConfig serverConfig;

	std::set<HttpMethod> methods;
	methods.insert(HttpMethod::GET);
	methods.insert(HttpMethod::POST);

	std::vector<Location> locations;
	locations.push_back(Location("/", "root/", false, methods));
	locations.push_back(Location("/images", "images/", false, methods));
	locations.push_back(Location("/api", "api/", false, methods));

	Server server("Server");
	server.setLocations(locations);
	serverConfig.addServer(server);

	// When
	const Location rootLoc = serverConfig.getServers()[0].findLocation("/");
	const Location imgLoc = serverConfig.getServers()[0].findLocation("/images/cat.png");
	const Location apiLoc = serverConfig.getServers()[0].findLocation("/api/users");

		// Than
	CHECK(rootLoc.path_prefix == "/");
	CHECK(imgLoc.path_prefix == "/images");
	CHECK(apiLoc.path_prefix == "/api");
	CHECK_THROWS(serverConfig.getServers()[0].findLocation("/doesnotexist"));
}
