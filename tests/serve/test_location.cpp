/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_location.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/18 14:57:40 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/18 15:09:52 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "tests/doctest.h"

#include "config/Location.hpp"

TEST_CASE("Location::hasUploadsDir() success")
{
	Location location;
	location.addUploadDirectory("tests");

	const bool could_open = location.hasUploadsDir(location.upload_dir);
	CHECK(could_open == true);
}

TEST_CASE("Location::hasUploadsDir() failure")
{
	Location location;
	location.addUploadDirectory("doesNotExist");

	const bool could_open = location.hasUploadsDir(location.upload_dir);
	CHECK(could_open == false);
}
