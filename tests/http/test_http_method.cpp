/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_http_method.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 10:04:46 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/26 17:16:36 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "http/HttpMethod.hpp"
#include "tests/doctest.h"

TEST_CASE("HttpMethod: to_method basic")
{
	CHECK(to_method("GET") == HttpMethod::GET);
	CHECK(to_method("POST") == HttpMethod::POST);
	CHECK(to_method("DELETE") == HttpMethod::DELETE_);
	CHECK(to_method("X") == HttpMethod::UNKNOWN);
}

TEST_CASE("HttpMethod: to_string basic")
{
	CHECK(to_string(HttpMethod::GET) == "GET");
	CHECK(to_string(HttpMethod::DELETE_) == "DELETE");
}
