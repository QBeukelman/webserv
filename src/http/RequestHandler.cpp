/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 13:13:04 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/03 13:14:24 by quentinbeuk   ########   odam.nl         */
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
	const Location *location = serverConfig.findLocation(request.path);

	switch (request.method)
	{
	case HttpMethod::GET:
		return (handleGet(request, *location));
	case HttpMethod::POST:
		return (handlePost(request, *location));
	case HttpMethod::DELETE:
		return (handlePost(request, *location));
	default:
		return (makeError(HttpStatus::STATUS_BAD_REQUEST, "HttpMethod not found"));
	}
	return (makeError(HttpStatus::STATUS_BAD_REQUEST, "HttpMethod not found"));
}

// PRIVATE
// ____________________________________________________________________________
HttpResponse RequestHandler::makeError(int status, std::string detail) const
{
	std::ostringstream oss;
	oss << status << ": " << detail;
	Logger::error(oss.str());

	// TODO: Define Response
	return (HttpResponse());
}

HttpResponse RequestHandler::handleGet(const HttpRequest &request, const Location &location) const
{

	std::cout << request;
	std::cout << location;

	// TODO: Define Response
	return (HttpResponse());
}

HttpResponse RequestHandler::handlePost(const HttpRequest &request, const Location &location) const
{

	std::cout << request;
	std::cout << location;

	// TODO: Define Response
	return (HttpResponse());
}

HttpResponse RequestHandler::handleDelete(const HttpRequest &request, const Location &location) const
{

	std::cout << request;
	std::cout << location;

	// TODO: Define Response
	return (HttpResponse());
}
