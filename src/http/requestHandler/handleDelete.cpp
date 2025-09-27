/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleDelete.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/23 08:26:48 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/27 11:24:59 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

/*
 * Delete a file
 *
 * Notes:
 * 	- Uses `::remove()` defined by the C standard library (C++ <cstdio>).
 */
HttpResponse RequestHandler::handleDelete(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleDelete()"));

	const std::string file_path = location.normalizeDirectory(location.getRoot()) + request.path;

	if (access(file_path.c_str(), F_OK) != 0)
	{
		Logger::error("RequestHandler::handleDelete → Forbidden path");
		return (makeError(STATUS_FORBIDDEN, "Forbidden path"));
	}

	if (::remove(file_path.c_str()) != 0)
	{
		Logger::error("RequestHandler::handleDelete → Could not delete file");
		return (makeError(STATUS_INTERNAL_ERROR, "Could not delete file"));
	}

	Logger::info("RequestHandler::handleDelete() → Delete successful");

	HttpResponse response;
	response.httpStatus = HttpStatus::STATUS_NO_CONTENT;
	response.headers["Content-Length"] = "0";

	return (response);
}
