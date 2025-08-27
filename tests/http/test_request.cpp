/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_request.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 08:59:11 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/27 16:53:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/Request.hpp"
#include "tests/doctest.h"
#include "tests/fixtures/test_http_request_data.hpp"

TEST_CASE("Parser: Simple GET without body")
{
	Request request(HTTP_REQUEST_GET);
	HttpRequest r = request.getRequest();

	CHECK(r.status == PARSE_OK);
	CHECK(r.method == HttpMethod::GET);
	CHECK(r.path == "/");
	CHECK(r.version == "HTTP/1.1");
	// CHECK(r.headers.find("host")->second == "a");
	CHECK(r.body == "");
}

TEST_CASE("Parser: POST with many headers")
{
	// Request request(HTTP_REQUEST_POST);
	// HttpRequest r = request.getRequest();

	// CHECK(r.status == PARSE_OK);
	// CHECK(r.method == HttpMethod::POST);
	// CHECK(r.version == "HTTP/1.1");

	// CHECK(r.path == "/submit-form");
	// CHECK(r.query == "debug=true");

	// // Headers (names assumed lowercased by parseHeaders)
	// REQUIRE(r.headers.find("host") != r.headers.end());
	// CHECK(r.headers["host"] == "www.example.com");

	// REQUIRE(r.headers.find("user-agent") != r.headers.end());
	// CHECK(r.headers["user-agent"] == "curl/7.68.0");

	// REQUIRE(r.headers.find("content-type") != r.headers.end());
	// CHECK(r.headers["content-type"] == "application/x-www-form-urlencoded");

	// REQUIRE(r.headers.find("content-length") != r.headers.end());
	// CHECK(r.headers["content-length"] == "26");

	// Body
	// CHECK(r.body == "name=Alice&age=30&city=AMS");
	// CHECK(r.body.size() == 26);
}
