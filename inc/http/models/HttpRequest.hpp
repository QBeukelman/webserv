/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequest.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 10:07:27 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/30 13:44:01 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "HttpMethod.hpp"
#include "RequestParseStatus.hpp"
#include "http/models/ContentType.hpp"
#include "utils/utils.hpp"

#include <iostream>
#include <map>
#include <string>

class HttpRequest
{
  public:
	HttpMethod method;	 // "GET", "POST", "DELETE"
	std::string target;	 // "/path?query=..."
	std::string path;	 // "/path"
	std::string query;	 // "a=1&b=2"
	std::string version; // "HTTP/1.1"
	std::string body;
	ContentType content_type;
	unsigned short port; // For CGI
	std::map<std::string, std::string> headers;
	RequestParseStatus status = RequestParseStatus::PARSE_INCOMPLETE;

	// Search headers
	std::string searchHeader(const std::string &key) const;
	std::string getHostName() const;
	unsigned short getHostPort() const;
};

std::ostream &operator<<(std::ostream &out, const HttpRequest &req);

#endif
