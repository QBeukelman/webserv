/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 12:25:32 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/03 12:03:43 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "config/Location.hpp"
#include "config/ServerConfig.hpp"
#include "http/RequestParser.hpp"
#include "http/models/HttpMethod.hpp"
#include "http/models/HttpRequest.hpp"
#include "http/models/HttpResponse.hpp"
#include "http/models/HttpStatus.hpp"
#include "http/models/RequestParseStatus.hpp"
#include "log/Logger.hpp"

#include <iostream>
#include <string>

class RequestHandler
{
  private:
	HttpResponse handleGet(const HttpRequest &, const Location &) const;
	HttpResponse handlePost(const HttpRequest &, const Location &) const;
	HttpResponse handleDelete(const HttpRequest &, const Location &) const;
	HttpResponse makeError(int status, std::string detail) const;

  public:
	const ServerConfig serverConfig;

	explicit RequestHandler(const ServerConfig &);
	HttpResponse handle(const HttpRequest &request) const;
};

#endif
