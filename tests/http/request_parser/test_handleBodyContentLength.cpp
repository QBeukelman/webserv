/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_handleBodyContentLength.cpp                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/01 13:01:00 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/12 20:01:24 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "tests/doctest.h"

#include "http/RequestParser.hpp"
#include "tests/fixtures/test_http_request_data.hpp"

static ParseContext make_ctx(size_t content_len, size_t max_body = 1 << 20)
{
	ParseContext ctx;
	ctx.phase = READ_BODY_CONTENT_LENGTH;
	ctx.read_offset = 0;
	ctx.content_length_remaining = content_len;
	ctx.total_body_bytes = 0;
	ctx.limits.max_body_size = max_body;
	return (ctx);
}

TEST_CASE("handleBodyContentLength: zero Content-Length completes immediately")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	RequestParser p(HttpRequestLimits{});
	ParseContext ctx = make_ctx(0);
	const char *data = "";
	ParseStep step = p.handleBodyContentLength(ctx, data, 0);

	CHECK(step.request_complete == true);
	CHECK(step.need_more == false);
	CHECK(step.status == PARSE_OK);
	CHECK(step.consumed == 0);
	CHECK(ctx.phase == COMPLETE);
}

TEST_CASE("handleBodyContentLength: no bytes available -> incomplete, need more")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	RequestParser p(HttpRequestLimits{});
	ParseContext ctx = make_ctx(5);

	// Simulate a window where read_offset == len (no new bytes)
	ctx.read_offset = 4;
	const char *data = "XXXX";
	ParseStep step = p.handleBodyContentLength(ctx, data, 4);

	CHECK(step.status == PARSE_INCOMPLETE);
	CHECK(step.need_more == true);
	CHECK(step.request_complete == false);
	CHECK(step.consumed == 0);
	CHECK(ctx.content_length_remaining == 5);
	CHECK(ctx.request.body.empty());
}

TEST_CASE("handleBodyContentLength: partial copy respects remaining length")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	RequestParser p(HttpRequestLimits{});
	ParseContext ctx = make_ctx(10);
	const char *data = "ABCDEFGHIJ";

	// 1st call copies some bytes (short window)
	// Set read_offset so available = 4
	ctx.read_offset = 6; // len(10) - 6 = 4 available
	ParseStep s1 = p.handleBodyContentLength(ctx, data, 10);

	CHECK(s1.status == PARSE_INCOMPLETE);
	CHECK(s1.need_more == true);
	CHECK(s1.request_complete == false);
	CHECK(s1.consumed == 4);
	CHECK(ctx.request.body == "GHIJ");
	CHECK(ctx.content_length_remaining == 6);

	const char *data2 = "0123456789";
	ctx.read_offset = 0;
	ParseStep s2 = p.handleBodyContentLength(ctx, data2, 6);

	CHECK(s2.status == PARSE_OK);
	CHECK(s2.request_complete == true);
	CHECK(s2.need_more == false);
	CHECK(s2.consumed == 6);
	CHECK(ctx.request.body == std::string("GHIJ") + "012345");
	CHECK(ctx.phase == COMPLETE);
}

TEST_CASE("handleBodyContentLength: enforces max_body_size across calls")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	RequestParser p(HttpRequestLimits{});
	ParseContext ctx = make_ctx(10, 8);

	const char *data = "AAAAAA";
	ParseStep s1 = p.handleBodyContentLength(ctx, data, 6);
	CHECK(s1.status == PARSE_INCOMPLETE);
	CHECK(s1.consumed == 6);
	CHECK(ctx.total_body_bytes == 6);
	CHECK(ctx.content_length_remaining == 4);

	const char *data2 = "BBBB";
	ParseStep s2 = p.handleBodyContentLength(ctx, data2, 4);
	CHECK(s2.status == PARSE_EXCEED_BODY_LIMIT);
	CHECK(ctx.phase == ERROR_PHASE);
}

TEST_CASE("handleBodyContentLength: does not read past current window (available)")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	RequestParser p(HttpRequestLimits{});
	ParseContext ctx = make_ctx(20);
	const char *data = "0123456789";
	ctx.read_offset = 7;

	ParseStep step = p.handleBodyContentLength(ctx, data, 10);
	CHECK(step.consumed == 3);
	CHECK(ctx.request.body == "789");
	CHECK(ctx.content_length_remaining == 17);
	CHECK(step.status == PARSE_INCOMPLETE);
	CHECK(step.need_more == true);
}
