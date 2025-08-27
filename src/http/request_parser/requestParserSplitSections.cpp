/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   requestParserSplitSections.cpp                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 08:40:17 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/27 08:49:42 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

/**
 * @brief Split a raw HTTP/1.x request string into its three physical sections.
 *
 * This function performs the first stage of HTTP request parsing:
 *   1. Start-line (e.g. "GET /path HTTP/1.1")
 *   2. Header block (all header lines up to the empty line)
 *   3. Body (all remaining bytes after CRLFCRLF)
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
