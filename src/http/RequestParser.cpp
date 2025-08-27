/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParser.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:50:01 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/27 16:47:36 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

RequestParser::RequestParser(RequestLimits limits)
{
	this->limits = limits;
}

// PUBLIC
// ____________________________________________________________________________
RequestLimits RequestParser::getLimits(void)
{
	return (this->limits);
}

HttpRequest RequestParser::parse(std::string requestString)
{
	HttpRequest request;
	request.status = PARSE_INCOMPLETE;

	std::string startLine, headerBlock, bodySrc;
	RequestParseStatus status = PARSE_INCOMPLETE;

	if (splitSections(requestString, startLine, headerBlock, bodySrc, status) == false)
	{
		request.status = status;
		return (request);
	}

	if (parseStartLine(startLine, request, status) == false)
	{
		request.status = status;
		return (request);
	}

	if (parseHeaders(headerBlock, request.headers, status) == false)
	{
		request.status = status;
		return (request);
	}

	// Caller sets parse status
	request.status = PARSE_OK;

	return (request);
}
