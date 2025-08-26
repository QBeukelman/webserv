/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_request.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 08:59:11 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/26 11:54:05 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/Request.hpp"
#include "tests/doctest.h"
#include "tests/utils/test_read_file.hpp"

TEST_CASE("Parser: simple GET without body")
{

	const std::string raw = "GET / HTTP/1.1\r\nHost: a\r\n\r\n";
	Request request(raw);
	HttpRequest requestObject = request.getRequest();

	CHECK(requestObject.status == PARSE_OK);
	// CHECK(requestObject.method == HttpMethod::GET);
	// CHECK(request.path == "/");
	// CHECK(request.version == "HTTP/1.1");
	// CHECK(request.headers.find("host")->second == "a");
	CHECK(requestObject.body == "");
}

// TEST_CASE("Parser: simple GET with body")
// {
// std::string data;
// REQUIRE_NOTHROW(data = read_file("tests/fixtures/http_request_large.txt"));

// 	RequestParser parser(RequestLimits{});

// 	CHECK(true);
// }
