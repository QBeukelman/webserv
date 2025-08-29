/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 13:13:04 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/29 10:06:00 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
	*Parsing checklist (must-haves)

		- Split headers/body at first \r\n\r\n. Reject if headers exceed limits.

	*Start line: METHOD SP TARGET SP HTTP/1.1\r\n

		- Validate method (GET/POST/DELETE at minimum).
		- Extract path and query from target.
		- Version must be HTTP/1.1.

	*Headers:

		- Case-insensitive keys; trim spaces.
		- Require Host (HTTP/1.1).
		- Determine keep_alive (default true unless Connection: close).

	*Body:

		- If Transfer-Encoding: chunked → decode chunks until size 0.
		- Else if Content-Length → read exactly that many bytes.
		- Enforce client_max_body_size → 413 if exceeded.

	*Common errors:

		- 400 bad syntax/invalid header format/missing Host.
		- 405 method not allowed (later in handler per location).
		- 411 length required (body without length & not chunked).
		- 414 URI too long.
		- 501 method not implemented.
*/

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
