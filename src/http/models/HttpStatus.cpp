/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpStatus.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 15:23:10 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/01 13:22:19 by quentinbeuk   ########   odam.nl         */
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
	case STATUS_REQUEST_TIMEOUT:
		return "Request timeout";
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

HttpStatus toHttpStatus(int code)
{
	switch (code)
	{
	// 1xx Informational
	case 100:
		return STATUS_CONTINUE;
	case 101:
		return STATUS_SWITCHING_PROTOCOL;

	// 2xx Success
	case 200:
		return STATUS_OK;
	case 201:
		return STATUS_CREATED;
	case 204:
		return STATUS_NO_CONTENT;

	// 3xx Redirection
	case 301:
		return STATUS_MOVED_PERMANENTLY;
	case 302:
		return STATUS_FOUND;
	case 303:
		return STATUS_SEE_OTHER;
	case 304:
		return STATUS_NOT_MODIFIED;
	case 307:
		return STATUS_TEMPORARY_REDIRECT;

	// 4xx Client errors
	case 400:
		return STATUS_BAD_REQUEST;
	case 401:
		return STATUS_UNAUTHORIZED;
	case 403:
		return STATUS_FORBIDDEN;
	case 404:
		return STATUS_NOT_FOUND;
	case 405:
		return STATUS_METHOD_NOT_ALLOWED;
	case 408:
		return STATUS_REQUEST_TIMEOUT;
	case 413:
		return STATUS_PAYLOAD_TOO_LARGE;

	// 5xx Server errors
	case 500:
		return STATUS_INTERNAL_ERROR;
	case 501:
		return STATUS_NOT_IMPLEMENTED;
	case 502:
		return STATUS_BAD_GATEWAY;
	case 503:
		return STATUS_SERVICE_UNAVAILABLE;

	default:
		// Fallback:
		return STATUS_INTERNAL_ERROR;
	}
}

std::string toStatusLine(HttpStatus status, const std::string &version)
{
	std::ostringstream oss;
	oss << version << " " << static_cast<int>(status) << " " << reasonPhrase(status);
	return (oss.str());
}

std::ostream &operator<<(std::ostream &out, HttpStatus status)
{
	out << static_cast<int>(status) << " " << reasonPhrase(status);
	return (out);
}
