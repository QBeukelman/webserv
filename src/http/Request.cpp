/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 13:13:04 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/01 20:01:09 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/Request.hpp"
#include "http/RequestParser.hpp"
#include "log/Logger.hpp"

// PRIVATE
// ____________________________________________________________________________
RequestParseStatus Request::parse(const char *data)
{
	Logger::info("Parse called");
	RequestParser p(HttpRequestLimits{});

	this->step = p.step(context, data, p.strLen(data));
	return (RequestParseStatus());
}

// CONSTRUCTORS
// ____________________________________________________________________________
Request::Request(const char *data)
{
	Logger::info("Request constructor called");
	this->step = ParseStep();
	this->context = ParseContext();
	parse(data);
}

ParseStep Request::getParserStep(void) const
{
	return (this->step);
}

void Request::setParserStep(ParseStep newStep)
{
	this->step = newStep;
}

ParseContext Request::getParseContext(void) const
{
	return (this->context);
}

void Request::setParserContext(ParseContext newContext)
{
	this->context = newContext;
}

// EXECUTE
// ____________________________________________________________________________
HttpResponse Request::execute() const
{
	HttpResponse response = HttpResponse();
	return (response);
}
