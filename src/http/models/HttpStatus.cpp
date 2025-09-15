/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpStatus.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 15:23:10 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/09 11:23:54 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/HttpStatus.hpp"

const char *reasonPhrase(HttpStatus code)
{
	switch (code)
	{
	// 1xx Informational
	case STATUS_CONTINUE:
		return "Continue";
	case STATUS_SWITCHING_PROTOCOL:
		return "Switching Protocols";

	// 2xx Success
	case STATUS_OK:
		return "OK";
	case STATUS_CREATED:
		return "Created";
	case STATUS_NO_CONTENT:
		return "No Content";

	// 3xx Redirection
	case STATUS_MOVED_PERMANENTLY:
		return "Moved Permanently";
	case STATUS_FOUND:
		return "Found";
	case STATUS_SEE_OTHER:
		return "See Other";
	case STATUS_NOT_MODIFIED:
		return "Not Modified";
	case STATUS_TEMPORARY_REDIRECT:
		return "Temporary Redirect";

	// 4xx Client errors
	case STATUS_BAD_REQUEST:
		return "Bad Request";
	case STATUS_UNAUTHORIZED:
		return "Unauthorized";
	case STATUS_FORBIDDEN:
		return "Forbidden";
	case STATUS_NOT_FOUND:
		return "Not Found";
	case STATUS_METHOD_NOT_ALLOWED:
		return "Method Not Allowed";
	case STATUS_PAYLOAD_TOO_LARGE:
		return "Payload Too Large";

	// 5xx Server errors
	case STATUS_INTERNAL_ERROR:
		return "Internal Server Error";
	case STATUS_NOT_IMPLEMENTED:
		return "Not Implemented";
	case STATUS_BAD_GATEWAY:
		return "Bad Gateway";
	case STATUS_SERVICE_UNAVAILABLE:
		return "Service Unavailable";

	default:
		return "Unknown Status";
	}
}

std::string toStatusLine(HttpStatus status, const std::string &version)
{
	std::ostringstream oss;
	oss << version << " " << static_cast<int>(status) << " " << reasonPhrase(status) << "\r\n";
	return (oss.str());
}

std::ostream &operator<<(std::ostream &out, HttpStatus status)
{
	out << static_cast<int>(status) << " " << reasonPhrase(status);
	return (out);
}
