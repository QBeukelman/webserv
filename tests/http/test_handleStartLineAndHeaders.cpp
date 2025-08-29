/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_handleStartLineAndHeaders.cpp                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 11:35:30 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/29 10:48:47 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "http/RequestParser.hpp"
#include "tests/doctest.h"
#include "tests/fixtures/test_http_request_data.hpp"

TEST_CASE("RequestParser: handleStartLineAndHeaders()")
{
	// Given
	RequestParser p(HttpRequestLimits{});
	ParseStep step;
	ParseContext context;

	// When
	step = p.handleStartLineAndHeaders(context, HTTP_REQUEST_POST, 185);

	// Start-line
	CHECK(toStringMethod(context.request.method) == "POST");
	CHECK(context.request.target == "/submit-form?debug=true");
	CHECK(context.request.path == "/submit-form");
	CHECK(context.request.query == "debug=true");
	CHECK(context.request.version == "HTTP/1.1");

	// Headers
	CHECK(p.searchHeader(context.request.headers, "content-encoding") == "none");
	CHECK(p.searchHeader(context.request.headers, "content-length") == "26");
	CHECK(p.searchHeader(context.request.headers, "content-type") == "application/x-www-form-urlencoded");
	CHECK(p.searchHeader(context.request.headers, "host") == "www.example.com");
	CHECK(p.searchHeader(context.request.headers, "user-agent") == "curl/7.68.0");

	// TODO: Body
}
