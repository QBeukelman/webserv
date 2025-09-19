/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_generateUploadFile.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/19 11:56:36 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/19 12:02:44 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "tests/doctest.h"

#include "http/RequestHandler.hpp"

TEST_CASE("RequestHandler: generateUploadFile")
{
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

	Location location_root("/", "root/", false, methods);
	location_root.addUploadDirectory("/uploads");

	Location location_submit("/submit-form", "submit-form/", false, methods);
	location_root.addUploadDirectory("/uploads");

	locations.push_back(location_root);
	locations.push_back(location_submit);
	server.setLocations(locations);
	serverConfig.addServer(server);

	RequestHandler handler(server);

	std::string file_name = handler.generateUploadFile("/uploads");

	std::cout << "FileName: " << file_name << std::endl;
}
