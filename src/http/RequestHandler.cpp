/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 13:13:04 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/03 10:17:05 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"
#include "log/Logger.hpp"

// INIT
// ____________________________________________________________________________
RequestHandler::RequestHandler(const Server &newServer) : server(newServer)
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
HttpResponse RequestHandler::handle(const HttpRequest &request)
{
	std::cout << request << std::endl;

	std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + std::chrono::seconds(TIME_OUT);

	// 1) Validate location
	Location location;
	try
	{
		location = server.findLocation(request.path);
	}
	catch (Server::LocationNotFoundException &e)
	{
		Logger::error(Logger::join("RequestHandler::handle → ", e.what()));
		return (makeError(HttpStatus::STATUS_NOT_FOUND, "Location not found"));
	}

	if (std::chrono::steady_clock::now() > deadline)
		return makeError(HttpStatus::STATUS_SERVICE_UNAVAILABLE, "Processing timeout");

	// 2) Handle redirects
	if (location.has_redirects)
		return (generateRedirectResponse(location));

	switch (request.method)
	{
	case HttpMethod::GET:
		return (handleGet(request, location));
	case HttpMethod::POST:
		return (handlePost(request, location));
	case HttpMethod::DELETE:
		return (handleDelete(request, location));
	default:
		return (makeError(HttpStatus::STATUS_BAD_REQUEST, "HttpMethod not found"));
	}
	return (makeError(HttpStatus::STATUS_BAD_REQUEST, "HttpMethod not found"));
}

// PRIVATE
// ____________________________________________________________________________
/*
 * Search locations for allowed methods.
 */
const bool RequestHandler::isMethodAllowed(const HttpRequest &request, const Location &location) const
{
	std::cout << "Searhing Allowed Methods: " << std::endl;

	for (const auto &m : location.allowed_methods)
	{
		std::cout << toStringMethod(m) << std::endl;
	}

	return (location.allowed_methods.find(request.method) != location.allowed_methods.end());
}
