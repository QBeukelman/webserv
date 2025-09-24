/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleDelete.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/23 08:26:48 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/23 08:28:13 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

HttpResponse RequestHandler::handleDelete(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleDelete()"));

	// TODO: Define Response
	Logger::info("RequestHandler::handleDelete → Delete Accepted");
	return (HttpResponse());
}
