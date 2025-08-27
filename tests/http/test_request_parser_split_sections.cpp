/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_request_parser_split_sections.cpp             :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 11:35:30 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/27 08:50:01 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "http/RequestParser.hpp"
#include "tests/doctest.h"
#include "tests/fixtures/test_http_request_data.hpp"

// Helpers to make requests quickly
static std::string make_request(const std::string &start_line, const std::string &headers, const std::string &body = "")
{
	std::string req = start_line + "\r\n" + headers + "\r\n";
	req += body; // body follows the mandatory blank line
	return req;
}

TEST_CASE("RequestParser: SplitSections() simple GET without body")
{
	RequestParser p(RequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;

	CHECK(p.splitSections(HTTP_REQUEST_GET, sl, hb, b, st) == true);
	CHECK(sl == "GET / HTTP/1.1");
	CHECK(hb == "Host: a");
	CHECK(b == "");
	CHECK(st == PARSE_INCOMPLETE);
}

TEST_CASE("RequestParser: SplitSections() POST with body")
{
	RequestParser p(RequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;

	CHECK(p.splitSections(HTTP_REQUEST_POST, sl, hb, b, st) == true);
	CHECK(sl == "POST /submit-form?debug=true HTTP/1.1");
	CHECK(hb.find("Host: www.example.com\r\n") == 0);
	CHECK(hb.find("\r\nContent-Length: 26") != std::string::npos);
	CHECK(b == "name=Alice&age=30&city=AMS");
	CHECK(st == PARSE_INCOMPLETE);
}

TEST_CASE("RequestParser: SplitSections() POST no CRLF")
{
	RequestParser p(RequestLimits{});
	std::string sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;

	CHECK(p.splitSections(HTTP_REQUEST_POST_NO_CRLF, sl, hb, b, st) == false);
	CHECK(st == PARSE_INCOMPLETE);
}

TEST_CASE("RequestParser: Limits, start-line too long")
{
	// Very small limit to force a failure
	RequestLimits lim;
	lim.max_start_line = 16;
	lim.max_header_line = 8 * 1024;
	lim.max_header_size = 64 * 1024;

	RequestParser p(lim);

	// Start line > 16 bytes
	std::string long_target(100, 'A');
	std::string sl = "GET /" + long_target + " HTTP/1.1"; // clearly too long
	std::string headers = "Host: a\r\n";

	std::string out_sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	bool ok = p.splitSections(make_request(sl, headers), out_sl, hb, b, st);

	CHECK(ok == false);
	CHECK(st == PARSE_EXCEED_LIMIT);
}

TEST_CASE("RequestParser: Limits, single header line too long")
{
	RequestLimits lim;
	lim.max_start_line = 8 * 1024;
	lim.max_header_line = 32; // tiny header-line limit
	lim.max_header_size = 64 * 1024;

	RequestParser p(lim);

	// Header value longer than 32 bytes
	std::string long_val(200, 'X');
	std::string headers = "Host: a\r\n"
						  "User-Agent: " +
						  long_val + "\r\n";

	std::string out_sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	bool ok = p.splitSections(make_request("GET / HTTP/1.1", headers), out_sl, hb, b, st);

	CHECK(ok == false);
	CHECK(st == PARSE_EXCEED_LIMIT);
}

TEST_CASE("RequestParser: Limits, total header block too large")
{
	RequestLimits lim;
	lim.max_start_line = 8 * 1024;
	lim.max_header_line = 8 * 1024;
	lim.max_header_size = 64; // tiny total header budget

	RequestParser p(lim);

	// Build many small headers that together exceed 64 bytes
	std::string headers;
	for (int i = 0; i < 10; ++i)
	{
		headers += "X-H" + std::string(1, '0' + (i % 10)) + ": x\r\n"; // ~7 bytes each
	}

	std::string out_sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	bool ok = p.splitSections(make_request("GET / HTTP/1.1", headers), out_sl, hb, b, st);

	CHECK(ok == false);
	CHECK(st == PARSE_EXCEED_LIMIT);
}

TEST_CASE("RequestParser: Limits, body too large (by Content-Length)")
{
	RequestLimits lim;
	lim.max_start_line = 8 * 1024;
	lim.max_header_line = 8 * 1024;
	lim.max_header_size = 64 * 1024;
	lim.max_body_size = 10;

	RequestParser p(lim);

	// Body of length 11 (exceeds limit 10)
	std::string body = "ABCDEFGHIJK";
	std::string headers = "Host: a\r\n"
						  "Content-Length: 11\r\n";

	std::string out_sl, hb, b;
	RequestParseStatus st = PARSE_INCOMPLETE;
	bool ok = p.splitSections(make_request("POST / HTTP/1.1", headers, body), out_sl, hb, b, st);

	CHECK(ok == false);
	CHECK(st == PARSE_EXCEED_LIMIT);
}
