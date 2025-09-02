/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequest.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 10:07:27 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/02 13:43:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "HttpMethod.hpp"
#include "RequestParseStatus.hpp"

#include <iostream>
#include <map>
#include <string>

/*
	HttpResponse: Immutable once parced
*/
struct HttpRequest
{
	HttpMethod method;	 // "GET", "POST", "DELETE"
	std::string target;	 // "/path?query=..."
	std::string path;	 // "/path"
	std::string query;	 // "a=1&b=2"
	std::string version; // "HTTP/1.1"
	std::string body;
	std::map<std::string, std::string> headers;
	RequestParseStatus status = RequestParseStatus::PARSE_INCOMPLETE;
};

std::ostream &operator<<(std::ostream &out, const HttpRequest &req);

#endif
