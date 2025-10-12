/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 12:25:32 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/10 10:48:43 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "config/Location.hpp"
#include "config/ServerConfig.hpp"
#include "config/models/CgiConfig.hpp"
#include "http/RequestParser.hpp"
#include "http/models/File.hpp"
#include "http/models/HttpMethod.hpp"
#include "http/models/HttpRequest.hpp"
#include "http/models/HttpResponse.hpp"
#include "http/models/HttpStatus.hpp"
#include "http/models/MultipartFile.hpp"
#include "http/models/RequestParseStatus.hpp"
#include "log/Logger.hpp"
#include "serve/CgiProcess.hpp"

#include <cerrno>
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#define TIME_OUT 100

struct DispatchResult
{
	enum Kind
	{
		DISPACTH_STATIC,
		DISPACTH_CGI
	} kind;

	HttpResponse response;
	CgiConfig cgi;
	Location location;
};

class RequestHandler
{
  private:
	const Server &server;

	// Helpers
	HttpResponse handleMultipartPost(const HttpRequest &, const Location &) const;
	HttpResponse generateRedirectResponse(const Location &) const;
	HttpResponse generateAutoIndexResponse(const std::string &) const;
	HttpStatus errorFromErrno(int error) const;

	// Handlers
	HttpResponse handleGet(const HttpRequest &, const Location &) const;
	HttpResponse handlePost(const HttpRequest &, const Location &) const;
	HttpResponse handleDelete(const HttpRequest &, const Location &) const;

// Utils
#ifdef UNIT_TEST
  public:
#else
  private:
#endif
	const MultipartFile composeMultiPartData(const HttpRequest &request) const;
	const File generateUploadFile(const std::string &upload_dir, const std::string file_name) const;
	const bool isMethodAllowed(const HttpRequest &request, const Location &location) const;

  public:
	// Constructors
	explicit RequestHandler(const Server &);

	// Handlers
	DispatchResult dispatch(const HttpRequest &request) const;
	HttpResponse handleStatic(const HttpRequest &request, const Location &location);
	HttpResponse makeError(HttpStatus status, std::string detail) const;
	std::string makeHtmlPageHeader(void) const;
};

#endif
