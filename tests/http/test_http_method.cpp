/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_http_method.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 10:04:46 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/24 10:59:19 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "tests/doctest.h"
#include "http/HttpMethod.hpp"

TEST_CASE("to_method basic") {
	CHECK(to_method("GET")    == HttpMethod::GET);
	CHECK(to_method("POST")   == HttpMethod::POST);
	CHECK(to_method("DELETE") == HttpMethod::DELETE_);
	CHECK(to_method("X")      == HttpMethod::UNKNOWN);
}

TEST_CASE("to_string basic") {
	CHECK(to_string(HttpMethod::GET)     == "GET");
	CHECK(to_string(HttpMethod::DELETE_) == "DELETE");
}
