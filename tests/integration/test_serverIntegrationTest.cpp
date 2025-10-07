/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_serverIntegrationTest.cpp                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/15 09:06:45 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/07 15:27:27 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "tests/doctest.h"

#include "TestConfigBuilder.hpp"
#include "config/ServerConfig.hpp"
#include "serve/WebServer.hpp"

TEST_CASE("Server Integration Test: Connect to Listener")
{
	// Given
	TestConfigBuilder builder;
	ServerConfig config =
		builder.listen("127.0.0.1", 0).new_root("/").new_prefix("/submit").allow(HttpMethod::POST).build();
	WebServer webServer(config);

	// Assert
	// 1) Is server bound to port?
	const unsigned short port = webServer.primaryPort();
	CHECK(port > 0);

	// 2) Try TCP connect. Is socket is listening?
	int cfd = ::socket(AF_INET, SOCK_STREAM, 0);
	REQUIRE(cfd >= 0);

	sockaddr_in sa{};
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	REQUIRE(::inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr) == 1);

	int rc = ::connect(cfd, reinterpret_cast<sockaddr *>(&sa), sizeof(sa));
	CHECK(rc == 0); // connect should succeed
	::close(cfd);
}

TEST_CASE("Server Integration Test: All")
{
	TestConfigBuilder builder;
	Logger::setLogLevel(LOG_LEVEL_DEBUG);
	Logger::setUseColors(true);

	CgiConfig py;
	py.extension = ".py";
	py.executable_path = "/usr/bin/python3";
	py.working_directory = "/var/www/scripts";

	std::set<HttpMethod> allowed;
	allowed.insert(HttpMethod::GET);
	allowed.insert(HttpMethod::POST);
	allowed.insert(HttpMethod::DELETE);

	// 1) Default site at "/"
	Location loc_root("/",		   // prefix
					  "./var/www", // root
					  false,	   // has_redirects
					  allowed);
	// loc_root.addIndexFile("index.html");
	loc_root.setAutoindex(true);

	// 2) CGI under "/scripts"
	Location loc_scripts("/scripts", "./var/www", false, {HttpMethod::GET, HttpMethod::POST});
	loc_scripts.addCgi(py);

	// 3) Uploads under "/uploads"
	Location loc_uploads("/uploads", "./var/www/uploads", false,
						 {HttpMethod::POST, HttpMethod::GET, HttpMethod::DELETE});
	loc_uploads.addUploadDirectory("./var/www/uploads");

	// 4) Redirect
	Redirect redirect(HttpStatus::STATUS_MOVED_PERMANENTLY, "./var/www");
	Location loc_redirect("/redirect", "./var/www/redirect", true,
						  {HttpMethod::POST, HttpMethod::GET, HttpMethod::DELETE});
	loc_redirect.setRedirect(redirect);

	ServerConfig config = builder.listen("127.0.0.1", 8080)
							  .addLocation(loc_root)
							  .addLocation(loc_scripts)
							  .addLocation(loc_uploads)
							  .addLocation(loc_redirect)
							  .build();

	WebServer webServer(config);

	// Check port
	unsigned short port = webServer.primaryPort();
	CHECK(port > 0);

	// std::cout << "=================================================================\n";
	// std::cout << "WebServer running on http://127.0.0.1:" << port << "/" << std::endl;

	// webServer.run();
}
