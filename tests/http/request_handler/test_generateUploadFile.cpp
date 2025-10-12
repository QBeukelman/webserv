/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_generateUploadFile.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/19 11:56:36 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/10 09:53:30 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "tests/doctest.h"

#include "http/RequestHandler.hpp"

static void deleteFile(const File file)
{
	if (file.getFd() >= 0)
	{
		::close(file.getFd());
	}
	::remove(file.getName().c_str());
}

TEST_CASE("RequestHandler: generateUploadFile")
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

	Location location_root("var/www", "/uploads", false, methods);
	location_root.addUploadDirectory("var/www/uploads");

	Location location_submit("/submit-form", "submit-form/", false, methods);
	location_root.addUploadDirectory("uploads");

	locations.push_back(location_root);
	locations.push_back(location_submit);
	server.setLocations(locations);
	serverConfig.addServer(server);

	RequestHandler handler(server);

	File file = handler.generateUploadFile("var/www/uploads", "test_file.pdf");
	CHECK(file.getFd() >= 0);
	CHECK(file.getName() == "var/www/uploads/test_file.pdf");

	deleteFile(file);
}
