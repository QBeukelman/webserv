/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_findLocation.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:59 by quentinbeuk       #+#    #+#             */
/*   Updated: 2025/09/08 09:30:45 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests/doctest.h"

#include "config/ServerConfig.hpp"

TEST_CASE("ServerConfig: findLocation() basic matching")
{
	// Given
	ServerConfig server;

	std::set<HttpMethod> methods;
	methods.insert(HttpMethod::GET);
	methods.insert(HttpMethod::POST);

	std::vector<Location> locations;
	locations.push_back(Location("/", "root/", false, methods));
	locations.push_back(Location("/images", "images/", false, methods));
	locations.push_back(Location("/api", "api/", false, methods));

	server.setLocations(locations);

	// When
	const Location *rootLoc = server.findLocation("/");
	const Location *imgLoc = server.findLocation("/images/cat.png");
	const Location *apiLoc = server.findLocation("/api/users");
	const Location *unknown = server.findLocation("/doesnotexist");

	// Then
	CHECK(rootLoc != NULL);
	CHECK(imgLoc != NULL);
	CHECK(apiLoc != NULL);
	CHECK(unknown == NULL);

	CHECK(rootLoc->path_prefix == "/");
	CHECK(imgLoc->path_prefix == "/images");
	CHECK(apiLoc->path_prefix == "/api");
}
