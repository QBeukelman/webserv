/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_serverIntegrationTest.cpp                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/15 09:06:45 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/18 10:39:38 by quentinbeuk   ########   odam.nl         */
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

	// std::cout << "WebServer listening on 127.0.0.1:" << port << "\n";
}

TEST_CASE("Server Integration Test: Run Loop")
{
	TestConfigBuilder builder;

	ServerConfig config =
		builder.listen("127.0.0.1", 8080).new_root("/").new_prefix("/submit").allow(HttpMethod::POST).build();
	WebServer webServer(config);

	// Check port
	unsigned short port = webServer.primaryPort();
	CHECK(port > 0);

	std::cout << "WebServer running on http://127.0.0.1:" << port << "/" << std::endl;

	// webServer.run();
}
