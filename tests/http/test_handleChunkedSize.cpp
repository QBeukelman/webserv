/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_handleChunkedSize.cpp                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/01 13:00:44 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/05 13:51:24 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define UNIT_TEST

#include "tests/doctest.h"

#include "http/RequestParser.hpp"
#include "tests/fixtures/test_http_request_data.hpp"

TEST_CASE("RequestParser: handleChunkedSize()")
{
	// Given
	RequestParser p(HttpRequestLimits{});
	ParseStep step;
	ParseContext context;

	// Phase: READ_HEADERS
	step = p.step(context, HTTP_REQUEST_POST_CHUNKED, 227);

	// Phase: READ_CHUNK_SIZE
	step = p.step(context, HTTP_REQUEST_POST_CHUNKED, 227);

	CHECK(context.is_chunked == true);
	CHECK(context.chunk_bytes_remaining == 7);
}
