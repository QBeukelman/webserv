/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleGet.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/23 08:26:52 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/23 08:27:59 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

HttpResponse RequestHandler::handleGet(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleGet()"));

	if (std::optional<CGI> cgi = location.getCgiByExtension(request.path))
	{
		// Run CGI(CGI)
		return (HttpResponse());
	}

	// TODO: Define Response
	Logger::info("RequestHandler::handleGet → Get Accepted");
	return (makeMock200(request));
}
