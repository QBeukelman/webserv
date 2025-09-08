/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_parserStartLine.cpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 17:01:33 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/03 14:08:30 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "tests/doctest.h"

#include "http/RequestParser.hpp"
#include "http/models/HttpRequestLimits.hpp"
#include "tests/fixtures/test_http_request_data.hpp"

TEST_CASE("RequestParser: parseStartLine() Success simple")
{
	RequestParser p(HttpRequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	HttpRequest request;

	CHECK(p.parseStartLine("GET / HTTP/1.1", request, st) == true);
	CHECK(request.method == HttpMethod::GET);
	CHECK(request.path == "/");
	CHECK(request.version == "HTTP/1.1");
	CHECK(st == PARSE_INCOMPLETE);
}

TEST_CASE("RequestParser: parseStartLine() Success with path")
{
	RequestParser p(HttpRequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	HttpRequest request;

	CHECK(p.parseStartLine("GET /docs/My%20File.pdf HTTP/1.1", request, st) == true);
	CHECK(request.method == HttpMethod::GET);
	CHECK(request.path == "/docs/My%20File.pdf");
	CHECK(request.version == "HTTP/1.1");
	CHECK(st == PARSE_INCOMPLETE);
}

TEST_CASE("RequestParser: parseStartLine() Invalid method")
{
	RequestParser p(HttpRequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	HttpRequest request;

	CHECK(p.parseStartLine("PUT / HTTP/1.1", request, st) == false);
	CHECK(request.method == HttpMethod::UNKNOWN);
	CHECK(st == PARSE_MALFORMED_REQUEST);
}

TEST_CASE("RequestParser: parseStartLine() Invalid path")
{
	RequestParser p(HttpRequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	HttpRequest request;

	CHECK(p.parseStartLine("GET docs/My%20File.pdf HTTP/1.1", request, st) == false);
	CHECK(request.method == HttpMethod::UNKNOWN);
	CHECK(request.path == "");
	CHECK(st == PARSE_MALFORMED_REQUEST);
}

TEST_CASE("RequestParser: parseStartLine() Invalid version")
{
	RequestParser p(HttpRequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	HttpRequest request;

	CHECK(p.parseStartLine("GET /docs/My%20File.pdf HTTP/1.2", request, st) == false);
	CHECK(request.method == HttpMethod::UNKNOWN);
	CHECK(request.path == "");
	CHECK(request.version == "");
	CHECK(st == PARSE_MALFORMED_REQUEST);
}
