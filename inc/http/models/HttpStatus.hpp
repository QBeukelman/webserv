/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpStatus.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 15:13:02 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/03 14:25:46 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUS_HPP
#define HTTPSTATUS_HPP

#include <iostream>
#include <sstream>
#include <string>

enum HttpStatus
{
	// 1xx Informational
	STATUS_CONTINUE = 100,
	STATUS_SWITCHING_PROTOCOL = 101,

	// 2xx Success
	STATUS_OK = 200,
	STATUS_CREATED = 201,
	STATUS_NO_CONTENT = 204,

	// 3xx Redirection
	STATUS_MOVED_PERMANENTLY = 301,
	STATUS_FOUND = 302,
	STATUS_SEE_OTHER = 303,
	STATUS_NOT_MODIFIED = 304,
	STATUS_TEMPORARY_REDIRECT = 307,

	// 4xx Client errors
	STATUS_BAD_REQUEST = 400,
	STATUS_UNAUTHORIZED = 401,
	STATUS_FORBIDDEN = 403,
	STATUS_NOT_FOUND = 404,
	STATUS_METHOD_NOT_ALLOWED = 405,
	STATUS_PAYLOAD_TOO_LARGE = 413,

	// 5xx Server errors
	STATUS_INTERNAL_ERROR = 500,
	STATUS_NOT_IMPLEMENTED = 501,
	STATUS_BAD_GATEWAY = 502,
	STATUS_SERVICE_UNAVAILABLE = 503
};

/*
 * Retuns:
 *	"HTTP/1.1 404 Not Found\r\n"
 */
std::string toStatusLine(HttpStatus status, const std::string &version = "HTTP/1.1");

std::ostream &operator<<(std::ostream &out, HttpStatus status);

#endif // HTTPSTATUS_HPP