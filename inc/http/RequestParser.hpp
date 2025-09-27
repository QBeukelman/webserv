/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParser.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:33:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/26 13:51:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "http/models/ContentType.hpp"
#include "http/models/HttpRequest.hpp"
#include "http/models/Mime.hpp"
#include "http/models/ParseContext.hpp"
#include "log/Logger.hpp"

#include <cstddef>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// Delimiters for request
const std::string CRLF("\r\n"); // Carriage Return + Line Feed
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

std::ostream &operator<<(std::ostream &out, ParseStep &step);

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

	ParseStep handleError(ParseContext &) const; // TODO: Will remove

  public:
	explicit RequestParser(HttpRequestLimits limits);

	std::string searchHeader(const std::map<std::string, std::string> &h, const std::string &key) const;
	HttpRequestLimits getLimits(void) const;
	size_t strLen(const char *data) const;

	// Incremental state-machine entry point (for chunked & non-chunked)
	ParseStep step(ParseContext &ctx, const char *data, size_t len) const;
};

#endif
