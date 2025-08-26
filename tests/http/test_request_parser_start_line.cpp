/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_request_parser_start_line.cpp                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 17:01:33 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/26 17:17:29 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "http/RequestParser.hpp"
#include "tests/doctest.h"
#include "tests/fixtures/test_http_request_data.hpp"

TEST_CASE("RequestParser: parseStartLine() simple")
{
	RequestParser p(RequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	HttpRequest request;

	CHECK(p.parseStartLine("GET / HTTP/1.1", request, st) == true);

	CHECK(st == PARSE_INCOMPLETE);
}
