/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParser.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:50:01 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/28 22:06:19 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

RequestParser::RequestParser(HttpRequestLimits limits)
{
	this->limits = limits;
}

HttpRequestLimits RequestParser::getLimits(void) const
{
	return (this->limits);
}

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

ParseStep RequestParser::step(ParseContext &ctx, const char *data, size_t len) const
{
	switch (ctx.phase)
	{
	case READ_START_LINE:
	case READ_HEADERS:
		return (handleStartLineAndHeaders(ctx, data, len));

	case READ_BODY_CONTENT_LENGTH:
		// return (handleBodyContentLength(ctx, data, len));

	case READ_CHUNK_SIZE:
		// return (handleChunkSize(ctx, data, len));

	case READ_CHUNK_DATA:
		// return (handleChunkData(ctx, data, len));

	case COMPLETE:
		// return (handleComplete(ctx));

	case ERROR_STATE:
	default:
		return (ParseStep());
	}
	// TODO: Fill ParseStep
	return (ParseStep());
}
