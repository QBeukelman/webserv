/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_request.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 08:59:11 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 12:55:47 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/Request.hpp"
#include "tests/doctest.h"
#include "tests/utils/test_read_file.hpp"

TEST_CASE("request basic")
{
	// Request req(read_file("../fixtures/http_request_large.txt"));

	// auto r = req.getRequest();

	// std::cout << req << std::endl;

	CHECK(true);
}
