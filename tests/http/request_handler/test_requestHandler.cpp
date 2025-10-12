/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_requestHandler.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 09:37:32 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/12 10:28:15 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tests/doctest.h"

#include "http/RequestHandler.hpp"

TEST_CASE("RequestHandler: handle()")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	// Request
	HttpRequest request;

	request.method = HttpMethod::POST;
	request.target = "/submit-form?debug=true";
	request.path = "/submit-form";
	request.query = "debug=true";
	request.version = "HTTP/1.1";

	// Server
	Server server("Server");
	ServerConfig serverConfig;

	std::set<HttpMethod> methods;
	methods.insert(HttpMethod::GET);
	std::vector<Location> locations;
	locations.push_back(Location("/", "root/", false, methods));
	locations.push_back(Location("/submit-form", "submit-form/", false, methods));
	server.setLocations(locations);

	serverConfig.addServer(server);

	RequestHandler handler(server);

	// Given
	HttpResponse response = handler.handleStatic(request, locations[1]);

	// std::cout << response;
}
