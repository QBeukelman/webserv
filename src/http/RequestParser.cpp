/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParser.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:50:01 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/01 20:09:40 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

std::ostream &operator<<(std::ostream &out, ParseStep &step)
{
	out << "\n=== ParseStep ==="
		<< "\nStatus: " << toStringStatus(step.status) << "\nConsumed: " << step.consumed
		<< "\nNeed more: " << step.need_more << "\nRequest complete " << step.request_complete << std::endl;
	return (out);
}

RequestParser::RequestParser(HttpRequestLimits limits)
{
	this->limits = limits;
}

HttpRequestLimits RequestParser::getLimits(void) const
{
	return (this->limits);
}

/*
 * Non case-insensitive header lookup
 *
 * Return:
 * 	- `Value` or "" if not found.
 */
std::string RequestParser::searchHeader(const std::map<std::string, std::string> &h, const std::string &key) const
{
	std::string key_l = toLower(key);

	for (std::map<std::string, std::string>::const_iterator it = h.begin(); it != h.end(); ++it)
	{
		if (toLower(it->first) == key_l)
		{
			return (it->second);
		}
	}
	return ("");
}

/*
 * Read request body data.
 *
 * Notes:
 *   - A Content-Length header means a non-chunked body.
 *   - A Transfer-Encoding header means chunked body:
 *       parse "<hex>[;ext]\r\n" bytes and call step() again for the data.
 *   - Having both Content-Length and Transfer-Encoding is an error.
 */
ParseStep RequestParser::step(ParseContext &ctx, const char *data, size_t len) const
{
	ParseStep step;

	switch (ctx.phase)
	{
	case READ_START_LINE:
	case READ_HEADERS:
		return (step = handleStartLineAndHeaders(ctx, data, len));

	case READ_BODY_CONTENT_LENGTH:
		return (step = handleBodyContentLength(ctx, data, len));

	case READ_CHUNK_SIZE:
		return (step = handleChunkSize(ctx, data, len));

	case READ_CHUNK_DATA:
		return (step = handleChunkData(ctx, data, len));

	case COMPLETE:
		return (step = handleComplete(ctx));

	case ERROR_STATE:
	default:
		return (step);
	}
	return (step);
}
