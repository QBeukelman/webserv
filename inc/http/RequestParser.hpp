/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParser.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:33:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/28 22:08:58 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "http/HttpRequest.hpp"
#include "http/ParseContext.hpp"
#include "log/Logger.hpp"

#include <cstddef>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// Delimiters for request
const std::string CRLF("\r\n");
const std::string HDR_END("\r\n\r\n");

struct ParseStep
{
	size_t consumed;
	RequestParseStatus status;
	bool need_more;
	bool request_complete;
	ParseStep() : consumed(0), status(PARSE_INCOMPLETE), need_more(true), request_complete(false)
	{
	}
};

class RequestParser
{
  private:
	HttpRequestLimits limits;

	std::string toLower(const std::string &s) const;
	std::string trim(const std::string &s) const;

#ifdef UNIT_TEST
  public:
#else
  private:
#endif
	bool parseStartLine(const std::string &startLine, HttpRequest &out, RequestParseStatus &status) const;

	bool parseHeaders(const std::string &headerBlock, std::map<std::string, std::string> &headers,
					  RequestParseStatus &status) const;

	ParseStep handleStartLineAndHeaders(ParseContext &, const char *, size_t) const;
	ParseStep handleBodyContentLength(ParseContext &, const char *, size_t) const;
	ParseStep handleChunkSize(ParseContext &, const char *, size_t) const;
	ParseStep handleChunkData(ParseContext &, const char *, size_t) const;
	ParseStep handleComplete(ParseContext &) const;
	ParseStep handleError(ParseContext &) const;

  public:
	explicit RequestParser(HttpRequestLimits limits);

	std::string searchHeader(const std::map<std::string, std::string> &h, const std::string &key) const;
	HttpRequestLimits getLimits(void) const;

	// Incremental state-machine entry point (for chunked & non-chunked)
	ParseStep step(ParseContext &ctx, const char *data, size_t len) const;
};

#endif
