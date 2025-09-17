/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:13:04 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/17 14:30:04 by qbeukelm         ###   ########.fr       */
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
static bool isMethodAllowed(const HttpRequest &request, const Location &location)
{
	return (location.allowed_methods.find(request.method) != location.allowed_methods.end());
}

HttpResponse RequestHandler::makeError(HttpStatus status, std::string detail) const
{
	// Log error
	Logger::error("RequestHandler::" + detail + " → [" + std::to_string(status) + "] " + reasonPhrase(status));

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
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleGet()"));

	if (std::optional<CGI> cgi = location.getCgiByExtension(request.path))
	{
		// Run CGI(CGI)
		return (HttpResponse());
	}

	// TODO: Define Response
	Logger::info("RequestHandler::handleGet → Get Accepted");
	return (HttpResponse());
}

HttpResponse RequestHandler::handlePost(const HttpRequest &request, const Location &location) const
{

	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handlePost()"));

	// TODO: RequestHandler::handlePost() Body is not working?
	Logger::info("RequestHandler::handlePost → Post Accepted");
	std::cout << request;
	return (HttpResponse());
}

HttpResponse RequestHandler::handleDelete(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleDelete()"));

	// TODO: Define Response
	Logger::info("RequestHandler::handleDelete → Delete Accepted");
	return (HttpResponse());
}
