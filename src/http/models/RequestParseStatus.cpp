/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParseStatus.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:20:19 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/12 20:02:11 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/RequestParseStatus.hpp"

std::string toStringStatus(RequestParseStatus status)
{
	switch (status)
	{
	case PARSE_OK:
		return "OK";
	case PARSE_INTERNAL_ERROR:
		return "Internal server error";
	case PARSE_INVALID_METHOD:
		return "Invalid method";
	case PARSE_INVALID_VERSION:
		return "Invalid version";
	case PARSE_MALFORMED_REQUEST:
		return "Malformed request";
	case PARSE_EXCEED_BODY_LIMIT:
		return "Request exceeds body limit";
	case PARSE_EXCEED_STARTLINE_LIMIT:
		return "Request exceeds start-line limit";
	case PARSE_INCOMPLETE:
		return "Parse incomplete";
	default:
		return "Unknown parse status";
	}
}
