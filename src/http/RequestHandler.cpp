/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 13:13:04 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/28 14:41:57 by quentinbeuk   ########   odam.nl         */
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
HttpResponse RequestHandler::handle(const HttpRequest &request) const
{
	std::cout << request << std::endl;

	Location location;
	try
	{
		std::cout << "Request path: " << request.path << std::endl;
		location = server.findLocation(request.path);
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
	return (location.allowed_methods.find(request.method) != location.allowed_methods.end());
}

// PUBLIC
// ____________________________________________________________________________
// ==== Handlers ====
HttpResponse RequestHandler::makeError(HttpStatus status, std::string detail) const
{
	// Log error
	Logger::error("RequestHandler::makeError() → " + detail + " → [" + std::to_string(status) + "] " +
				  reasonPhrase(status));

	// Create error response
	HttpResponse response;
	response.httpStatus = status;

	response.headers["Content-Type"] = "text/html; charset=UTF-8";
	// TODO: HttpResponse html body
	response.body = "ERROR\r\n";
	response.headers["Content-Length"] = std::to_string(response.body.size());

	return (response);
}
