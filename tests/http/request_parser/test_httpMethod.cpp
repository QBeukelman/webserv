/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_httpMethod.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 10:04:46 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/10 09:54:46 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "tests/doctest.h"

#include "http/models/HttpMethod.hpp"
#include "log/Logger.hpp"

TEST_CASE("HttpMethod: toMethod() basic")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	CHECK(toMethod("GET") == HttpMethod::GET);
	CHECK(toMethod("POST") == HttpMethod::POST);
	CHECK(toMethod("DELETE") == HttpMethod::DELETE);
	CHECK(toMethod("X") == HttpMethod::UNKNOWN);
}

TEST_CASE("HttpMethod: toString() basic")
{
	Logger::setLogLevel(LOG_LEVEL_NONE);

	CHECK(toStringMethod(HttpMethod::GET) == "GET");
	CHECK(toStringMethod(HttpMethod::DELETE) == "DELETE");
}
