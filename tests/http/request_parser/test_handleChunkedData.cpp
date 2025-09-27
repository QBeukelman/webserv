/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_handleChunkedData.cpp                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/01 16:29:15 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/26 11:16:40 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "tests/doctest.h"

#include "http/RequestParser.hpp"
#include "tests/fixtures/test_http_request_data.hpp"

TEST_CASE("RequestParser: handleChunkedData()")
{
	// Given
	RequestParser p(HttpRequestLimits{});
	ParseStep step;
	ParseContext context;

	// Phase: READ_HEADERS
	step = p.step(context, HTTP_REQUEST_POST_CHUNKED, 210);

	// Phase: READ_CHUNK_SIZE
	step = p.step(context, HTTP_REQUEST_POST_CHUNKED, 210);

	// Phase: READ_CHUNK_DATA
	step = p.step(context, HTTP_REQUEST_POST_CHUNKED, 210);

	CHECK(context.is_chunked == true);
	CHECK(context.request.body == "Mozilla");
}

TEST_CASE("RequestParser: handleChunkedData() with loop")
{
	// Given
	RequestParser p(HttpRequestLimits{});
	ParseContext context;
	ParseStep step;

	// A fixed raw request (chunked). Normally from recv()
	const char *raw = HTTP_REQUEST_POST_CHUNKED;
	size_t total_len = 210;

	// Feed data until complete
	while (context.read_offset < total_len && context.request.status != PARSE_OK)
	{
		step = p.step(context, raw, total_len);
		if (context.phase == ERROR_PHASE)
			break;
	}

	CHECK(step.request_complete == true);
	CHECK(step.status == PARSE_OK);
	CHECK(context.phase == COMPLETE);
	CHECK(context.request.body == "MozillaDeveloperNetwork");
	CHECK(context.read_offset == (total_len - 1));
}
