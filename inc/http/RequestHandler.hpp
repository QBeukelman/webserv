/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:25:32 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/10/02 10:14:56 by qbeukelm         ###   ########.fr       */
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
#include "http/models/MultipartFile.hpp"
#include "http/models/RequestParseStatus.hpp"
#include "log/Logger.hpp"

#include <cerrno>
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

#define TIME_OUT 10

class RequestHandler
{
  private:
	const Server &server;

	// Helpers
	HttpResponse handleCgi(const HttpRequest &, const Location &, const CGI &) const;
	HttpResponse handleMultipartPost(const HttpRequest &, const Location &) const;
	HttpResponse generateRedirectResponse(const Location &) const;
	HttpResponse generateAutoIndexResponse(const std::string &) const;

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
	HttpResponse handle(const HttpRequest &request);
	HttpResponse makeError(HttpStatus status, std::string detail) const;
	std::string makeHtmlPageHeader(void) const;
};

#endif
