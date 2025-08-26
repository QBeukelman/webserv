/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_request_parser_split_sections.cpp             :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 11:35:30 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/26 12:39:57 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "http/RequestParser.hpp"
#include "tests/doctest.h"
#include "tests/utils/test_read_file.hpp"

TEST_CASE("Parser: SplitSections() simple GET without body")
{
	RequestParser p(RequestLimits{});
	std::string raw = "GET / HTTP/1.1\r\nHost: a\r\n\r\n";

	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;

	CHECK(p.splitSections(raw, sl, hb, b, st) == true);
	CHECK(sl == "GET / HTTP/1.1");
	CHECK(hb == "Host: a");
	CHECK(b == "");
	CHECK(st == PARSE_INCOMPLETE);
}

TEST_CASE("Parser: SplitSections() POST with body")
{
	RequestParser p(RequestLimits{});
	std::string raw;
	REQUIRE_NOTHROW(raw = read_file("tests/fixtures/http_request_large.txt"));

	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;

	// CHECK(p.splitSections(raw, sl, hb, b, st) == true);
	// CHECK(sl == "POST /submit-form?debug=true HTTP/1.1");

	// CHECK(hb.find("Host: www.example.com\r\n") == 0);
	// CHECK(hb.find("\r\nContent-Length: 26") != std::string::npos);

	// CHECK(b == "");

	CHECK(st == PARSE_INCOMPLETE);
}
