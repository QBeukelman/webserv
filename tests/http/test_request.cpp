/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_request.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 08:59:11 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/01 20:26:05 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/Request.hpp"
#include "tests/doctest.h"
#include "tests/fixtures/test_http_request_data.hpp"

// TODO: test_request()

TEST_CASE("Parser: Simple GET without body")
{
	// Request request(HTTP_REQUEST_GET);
	// HttpRequest r = request.getRequest();

	// CHECK(r.status == PARSE_OK);
	// CHECK(r.method == HttpMethod::GET);
	// CHECK(r.target == "/");
	// CHECK(r.path == "/");
	// CHECK(r.query == "");
	// CHECK(r.version == "HTTP/1.1");
	// CHECK(r.headers.find("host")->second == "a");
	// CHECK(r.body == "");
}

TEST_CASE("Parser: POST with many headers")
{
	// Request request(HTTP_REQUEST_POST);
	// HttpRequest r = request.getRequest();

	// CHECK(r.status == PARSE_OK);
	// CHECK(r.method == HttpMethod::POST);
	// CHECK(r.version == "HTTP/1.1");

	// CHECK(r.target == "/submit-form?debug=true");
	// CHECK(r.path == "/submit-form");
	// CHECK(r.query == "debug=true");

	// REQUIRE(r.headers.find("host") != r.headers.end());
	// CHECK(r.headers["host"] == "www.example.com");

	// REQUIRE(r.headers.find("user-agent") != r.headers.end());
	// CHECK(r.headers["user-agent"] == "curl/7.68.0");

	// REQUIRE(r.headers.find("content-type") != r.headers.end());
	// CHECK(r.headers["content-type"] == "application/x-www-form-urlencoded");

	// REQUIRE(r.headers.find("content-length") != r.headers.end());
	// CHECK(r.headers["content-length"] == "26");

	// CHECK(r.body == "name=Alice&age=30&city=AMS");
	// CHECK(r.body.size() == 26);
}
