/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParseStatus.hpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:19:11 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/12 20:01:48 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSESTATUS_HPP
#define REQUESTPARSESTATUS_HPP

#include <string>

enum RequestParseStatus
{
	PARSE_OK,
	PARSE_INTERNAL_ERROR,
	PARSE_INVALID_METHOD,
	PARSE_INVALID_VERSION,
	PARSE_MALFORMED_REQUEST,
	PARSE_EXCEED_BODY_LIMIT,
	PARSE_EXCEED_STARTLINE_LIMIT,
	PARSE_INCOMPLETE
};

std::string toStringStatus(RequestParseStatus status);

#endif
