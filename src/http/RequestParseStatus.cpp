/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParseStatus.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 09:20:19 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 09:20:55 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParseStatus.hpp"

std::string to_string(RequestParseStatus status)
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
	case PARSE_INCOMPLETE:
		return "Incomplete request";
	default:
		return "Unknown parse status";
	}
}
