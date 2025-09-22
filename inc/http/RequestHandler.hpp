/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 12:25:32 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/22 09:55:04 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "config/Location.hpp"
#include "config/ServerConfig.hpp"
#include "http/RequestParser.hpp"
#include "http/models/File.hpp"
#include "http/models/HttpMethod.hpp"
#include "http/models/HttpRequest.hpp"
#include "http/models/HttpResponse.hpp"
#include "http/models/HttpStatus.hpp"
#include "http/models/RequestParseStatus.hpp"
#include "log/Logger.hpp"

#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

#define MAX_UPLOAD_FILES 10000

class RequestHandler
{
  private:
	const Server &server;

	HttpResponse handleGet(const HttpRequest &, const Location &) const;
	HttpResponse handlePost(const HttpRequest &, const Location &) const;
	HttpResponse handleDelete(const HttpRequest &, const Location &) const;

// Utils
#ifdef UNIT_TEST
  public:
#else
  private:
#endif
	const File generateUploadFile(const std::string &upload_dir) const;
	const bool isMethodAllowed(const HttpRequest &request, const Location &location) const;

  public:
	// Constructors
	explicit RequestHandler(const Server &);

	// Handlers
	HttpResponse handle(const HttpRequest &request) const;
	HttpResponse makeError(HttpStatus status, std::string detail) const;
	HttpResponse makeMock200(const HttpRequest &request) const;
};

#endif
