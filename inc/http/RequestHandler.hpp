/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestHandler.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 12:25:32 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/02 13:53:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "config/ServerConfig.hpp"
#include "http/RequestParser.hpp"
#include "http/models/HttpMethod.hpp"
#include "http/models/HttpRequest.hpp"
#include "http/models/HttpResponse.hpp"
#include "http/models/RequestParseStatus.hpp"

#include <iostream>
#include <string>

class RequestHandler
{
  private:
	const ServerConfig serverConfig;

  public:
	explicit RequestHandler(const ServerConfig &);
	HttpResponse handle(const HttpRequest &request) const;

	const ServerConfig getConfig();
};

#endif
