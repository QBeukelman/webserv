/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 13:13:04 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/09 11:38:19 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"
#include "log/Logger.hpp"

// INIT
// ____________________________________________________________________________
RequestHandler::RequestHandler(const ServerConfig &newServerConfig) : serverConfig(newServerConfig)
{
}

// EXECUTE
// ____________________________________________________________________________
/*
 *	1) Config:
 * 		Check Request against ServerConfig.
 *
 * 	2) Routing:
 * 		Implement longest-prefix match over configured routs.
 *
 * 	3) Resolve Target on Disc:
 * 		Join `location.root` + `request.path`.
 *
 * 	4) Dispatch:
 * 		GET, POST, DELETE.
 *
 * 	5) CGI Hook:
 * 		Build env, dechunk...
 *
 * 	6) Error & Keep-Alive:
 * 		Keep alive or close based on request headers.
 */
HttpResponse RequestHandler::handle(const HttpRequest &request) const
{
	Location location;
	try
	{
		location = serverConfig.getServers()[0].findLocation(request.path);
	}
	catch (Server::LocationNotFoundException &e)
	{
		Logger::error(Logger::join("RequestHandler::handle → ", e.what()));
		return (makeError(HttpStatus::STATUS_NOT_FOUND, "Location not found"));
	}

	switch (request.method)
	{
	case HttpMethod::GET:
		return (handleGet(request, location));
	case HttpMethod::POST:
		return (handlePost(request, location));
	case HttpMethod::DELETE:
		return (handlePost(request, location));
	default:
		return (makeError(HttpStatus::STATUS_BAD_REQUEST, "HttpMethod not found"));
	}
	return (makeError(HttpStatus::STATUS_BAD_REQUEST, "HttpMethod not found"));
}

// PRIVATE
// ____________________________________________________________________________
static bool isMethodAllowed(const HttpRequest &request, const Location &location)
{
	return (location.allowed_methods.find(request.method) != location.allowed_methods.end());
}

HttpResponse RequestHandler::makeError(HttpStatus status, std::string detail) const
{
	// Log error
	Logger::error(Logger::join(reasonPhrase(status), detail));

	// Create error response
	HttpResponse response;
	response.httpStatus = status;

	// TODO: HttpResponse set `content-type` header
	response.headers["Content-Type"] = "text/html; charset=UTF-8";

	// TODO: HttpResponse html body
	response.body = "<html>\r\n";

	response.headers["Content-Length"] = std::to_string(response.body.size());

	return (response);
}

HttpResponse RequestHandler::handleGet(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleGet"));

	// TODO: Define Response
	return (HttpResponse());
}

HttpResponse RequestHandler::handlePost(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handlePost"));

	// TODO: Define Response
	return (HttpResponse());
}

HttpResponse RequestHandler::handleDelete(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleDelete"));

	// TODO: Define Response
	return (HttpResponse());
}
