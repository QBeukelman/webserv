/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParser.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:50:01 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/26 17:25:36 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"
#include "log/Logger.hpp"

// Delimiters (HTTP/1.x requires CRLF)
static const std::string CRLF("\r\n");
static const std::string HDR_END("\r\n\r\n");

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

	if (!splitSections(requestString, startLine, headerBlock, bodySrc, status))
	{
		request.status = status;
		return (request);
	}

	if (!parseStartLine(startLine, request, status))
	{
		request.status = status;
		return (request);
	}

	// Caller sets parse status
	request.status = PARSE_OK;

	return (request);
}

// METHODS
// ____________________________________________________________________________
/**
 * @brief Split a raw HTTP/1.x request string into its three physical sections.
 *
 * This function performs the first stage of HTTP request parsing:
 *   1. Extract the start-line (e.g. "GET /path HTTP/1.1")
 *   2. Extract the header block (all header lines up to the empty line)
 *   3. Extract the candidate body (all remaining bytes after CRLFCRLF)
 *
 * Behavior:
 *   - Enforces size limits on the start-line, header block, and body.
 *   - Does not modify or validate the contents beyond simple splitting.
 *   - Body is returned exactly as-is, including binary data (no trimming).
 *   - Does not set PARSE_OK; caller must finalize status after further parsing.
 *
 * @param raw        [in]  Full request string as received (no chunking).
 * @param startLine  [out] Substring containing the request start-line only.
 * @param headerBlock[out] Substring containing raw headers (without the final CRLFCRLF).
 * @param body       [out] Substring containing the remaining bytes after headers.
 * @param status     [out] Updated with parse status in case of error:
 *                           - PARSE_INCOMPLETE: missing CRLF or CRLFCRLF
 *                           - PARSE_EXCEED_LIMIT: exceeded configured limits
 *
 * @return true if the split was successful (sections extracted), false otherwise.
 *
 * Notes:
 *   - If the request is incomplete, `status` is set to PARSE_INCOMPLETE.
 *   - This function does not validate HTTP syntax (method, version, headers).
 *   - Line-based header length limits must be checked later in parseHeaders().
 */
bool RequestParser::splitSections(const std::string &raw, std::string &startLine, std::string &headerBlock,
								  std::string &body, RequestParseStatus &status) const
{
	startLine.erase();
	headerBlock.erase();
	body.erase();

	// 1. Empty input
	if (raw.empty())
	{
		status = PARSE_INCOMPLETE;
		Logger::error("RequestParser::splitSections() -> Empty raw");
		return (false);
	}

	// 2. Find end of start-line
	std::string::size_type ptr_sl = raw.find(CRLF);
	if (ptr_sl == std::string::npos)
	{
		status = PARSE_INCOMPLETE;
		Logger::error("RequestParser::splitSections() -> Did not find CRLF");
		return (false);
	}
	if (ptr_sl > limits.max_start_line)
	{
		status = PARSE_EXCEED_LIMIT;
		Logger::error("RequestParser::splitSections() -> Start-line exceeds limit");
		return (false);
	}
	startLine.assign(raw, 0, ptr_sl);

	// 3. Header block
	// Find end of header block
	// headerBlock is the bytes between the start-line CRLF and the CRLFCRLF
	const std::string::size_type search_from = ptr_sl + CRLF.size();
	std::string::size_type ptr_hdr_end = raw.find(HDR_END, search_from);
	if (ptr_hdr_end == std::string::npos)
	{
		status = PARSE_INCOMPLETE;
		Logger::error("RequestParser::splitSections() -> Did not find HDR_END");
		return (false);
	}

	const std::string::size_type hdr_begin = search_from;
	const std::string::size_type hdr_len = ptr_hdr_end - hdr_begin;
	if (hdr_len > limits.max_header_size)
	{
		Logger::error("RequestParser::splitSections() -> Header block exceeds limit");
		status = PARSE_EXCEED_LIMIT;
		return (false);
	}

	// Enforce per-header-line limit
	{
		std::string::size_type line_start = hdr_begin;
		while (line_start < ptr_hdr_end)
		{
			std::string::size_type eol = raw.find(CRLF, line_start);
			if (eol == std::string::npos || eol > ptr_hdr_end)
			{
				// Malformed / incomplete header line
				status = PARSE_INCOMPLETE;
				Logger::error("RequestParser::splitSections() -> Header line not terminated");
				return false;
			}
			const std::string::size_type line_len = eol - line_start;
			if (line_len > limits.max_header_line)
			{
				status = PARSE_EXCEED_LIMIT;
				Logger::error("RequestParser::splitSections() -> Header line exceeds limit");
				return false;
			}
			line_start = eol + CRLF.size();
		}
	}
	headerBlock.assign(raw, hdr_begin, hdr_len);

	// 4. Body
	// Everything after CRLFCRLF is the candidate body (do NOT trim)
	const std::string::size_type body_begin = ptr_hdr_end + HDR_END.size();
	if (body_begin <= raw.size())
	{
		body.assign(raw, body_begin, raw.size() - body_begin);
	}
	else
	{
		body.erase();
	}
	if (limits.max_body_size && body.size() > limits.max_body_size)
	{
		status = PARSE_EXCEED_LIMIT;
		Logger::error("RequestParser::splitSections() -> Body exceeds limit");
		return (false);
	}

	// SUCCCESS – leave st unchanged; caller will set PARSE_OK after downstream parsing
	return (true);
}

/*
	1. Validate that the line is well-formed according to HTTP/1.1.
	2. Extract the three required parts:
		Method (GET, POST, DELETE, ...)
		Request target (the path and optional query, e.g. /path?query=1)
		HTTP version (e.g. HTTP/1.1)
	3. Store them in the HttpRequest& out structure.
	4. Set the RequestParseStatus& st to indicate success or a specific error.
*/
bool RequestParser::parseStartLine(const std::string &startLine, HttpRequest &out, RequestParseStatus &status) const
{
	std::istringstream iss(startLine);
	std::vector<std::string> tokens;
	std::string word;

	while (iss >> word)
	{
		tokens.push_back(word);
	}
	if (tokens.size() != 3)
	{
		status = PARSE_MALFORMED_REQUEST;
		Logger::error("RequestParser::parseStartLine() -> Token count does not equal 3");
		return (false);
	}

	// for (auto t : tokens)
	// {
	// 	std::cout << t << std::endl;
	// }

	out.method = to_method(tokens[0]);

	return (true);
}
