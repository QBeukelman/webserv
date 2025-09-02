/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_httpMethod.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 10:04:46 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/02 13:57:03 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "http/models/HttpMethod.hpp"
#include "tests/doctest.h"

TEST_CASE("HttpMethod: toMethod() basic")
{
	CHECK(toMethod("GET") == HttpMethod::GET);
	CHECK(toMethod("POST") == HttpMethod::POST);
	CHECK(toMethod("DELETE") == HttpMethod::DELETE_);
	CHECK(toMethod("X") == HttpMethod::UNKNOWN);
}

TEST_CASE("HttpMethod: toString() basic")
{
	CHECK(toStringMethod(HttpMethod::GET) == "GET");
	CHECK(toStringMethod(HttpMethod::DELETE_) == "DELETE");
}
