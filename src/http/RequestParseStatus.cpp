/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParseStatus.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:20:19 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/27 10:49:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParseStatus.hpp"

std::string toStringStatus(RequestParseStatus status)
{
	switch (status)
	{
	case PARSE_OK:
		return "OK";
	case PARSE_INVALID_METHOD:
		return "Invalid method";
	case PARSE_INVALID_VERSION:
		return "Invalid version";
	case PARSE_MALFORMED_REQUEST:
		return "Malformed request";
	case PARSE_EXCEED_LIMIT:
		return "Request exceeds limit";
	case PARSE_INCOMPLETE:
		return "Incomplete request";
	default:
		return "Unknown parse status";
	}
}
