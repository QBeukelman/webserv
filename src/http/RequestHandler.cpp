/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 13:13:04 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/07 18:45:53 by quentinbeuk   ########   odam.nl         */
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
	return (location.allowed_methods.find(request.method) != location.allowed_methods.end());
}

/*
 * STATUS_FORBIDDEN `403`
 * STATUS_NOT_FOUND `404`
 * STATUS_INTERNAL_ERROR `500`
 */
HttpStatus RequestHandler::errorFromErrno(int error) const
{
	if (error == EACCES)
		return (STATUS_FORBIDDEN); // 403
	if (error == ENOENT || error == ENOTDIR)
		return (STATUS_NOT_FOUND);	// 404
	return (STATUS_INTERNAL_ERROR); // 500
}

DispatchResult RequestHandler::dispatch(const HttpRequest &request) const
{
	Location location;
	try
	{
		location = server.findLocation(request.path);
	}
	catch (Server::LocationNotFoundException &e)
	{
		Logger::error(Logger::join("RequestHandler::handle → ", e.what()));
		return {DispatchResult::DISPACTH_STATIC, makeError(HttpStatus::STATUS_NOT_FOUND, "Location not found")};
	}

	if (isMethodAllowed(request, location) == false)
		return {DispatchResult::DISPACTH_STATIC, makeError(STATUS_METHOD_NOT_ALLOWED, "method not allowed")};

	if (request.body.size() > location.getMaxBodySize())
		return {DispatchResult::DISPACTH_STATIC, makeError(STATUS_PAYLOAD_TOO_LARGE, "request body is too long")};

	if (std::optional<CgiConfig> found_cgi = location.getCgiByExtension(request.path))
		return {DispatchResult::DISPACTH_CGI, HttpResponse{}, found_cgi.value(), location};

	HttpResponse response = RequestHandler(this->server).handle(request);
	return {DispatchResult::DISPACTH_STATIC, response, {}, location};
}
