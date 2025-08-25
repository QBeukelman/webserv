/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:07:27 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 10:42:32 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "http/HttpMethod.hpp"
#include "http/RequestParseStatus.hpp"

#include <string>
#include <unordered_map>

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
	std::unordered_map<std::string, std::string> headers;
	RequestParseStatus status = RequestParseStatus::PARSE_INCOMPLETE;
};

#endif
