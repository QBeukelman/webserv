/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_requestParserStep.cpp                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 08:59:11 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/10 09:55:10 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tests/doctest.h"

#include "http/RequestHandler.hpp"
#include "tests/fixtures/test_http_request_data.hpp"

TEST_CASE("RequestParser: step() content length")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	// Given
	RequestParser p(HttpRequestLimits{});
	ParseStep step;
	ParseContext context;

	const char *raw = HTTP_REQUEST_POST_CONTENT_LEN;
	size_t total_len = 185;

	// When
	while (context.read_offset < total_len && context.request.status != PARSE_OK)
	{
		step = p.step(context, raw, total_len);
		if (context.phase == ERROR_PHASE)
			break;
	}

	// Then
	CHECK(step.request_complete == true);
	CHECK(step.status == PARSE_OK);
	CHECK(context.phase == COMPLETE);
	CHECK(context.request.body == "name=Alice&age=30&city=AMS");
	CHECK(context.read_offset == (total_len - 1));
}

TEST_CASE("RequestParser: step() chunked")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	// Given
	RequestParser p(HttpRequestLimits{});
	ParseContext context;
	ParseStep step;

	const char *raw = HTTP_REQUEST_POST_CHUNKED;
	size_t total_len = 210;

	// When
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
