/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_request_start_line_headers.cpp                :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 11:35:30 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/28 22:01:15 by quentinbeuk   ########   odam.nl         */
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

	// Then
	// CHECK()
}
