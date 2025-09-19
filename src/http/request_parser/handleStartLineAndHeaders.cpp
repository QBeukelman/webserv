/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleStartLineAndHeaders.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 20:39:56 by quentinbeuk       #+#    #+#             */
/*   Updated: 2025/09/17 12:00:44 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

static bool find_header_end(const char *data, size_t len, size_t &header_end)
{
	if (len < 4)
		return (false);
	for (size_t i = 0; i + 3 < len; ++i)
	{
		if (data[i] == '\r' && data[i + 1] == '\n' && data[i + 2] == '\r' && data[i + 3] == '\n')
		{
			header_end = i; // points to the first '\r' in CRLF
			return (true);
		}
	}
	return (false);
}

static bool find_start_line_end(const char *data, size_t header_end, size_t &sl_end)
{
	for (size_t i = 0; i + 1 <= header_end; i++)
	{
		if (data[i] == '\r' && data[i + 1] == '\n')
		{
			sl_end = i;
			break;
		}
	}
	if (sl_end == (size_t)-1)
	{
		// TODO: No end of start-line
		return (false);
	}
	return (true);
}

static bool check_single_header_limit(const char *data, size_t header_end, size_t sl_end, size_t max_header_line)
{
	size_t line_start = sl_end + 2;
	for (size_t i = line_start; i <= header_end; ++i)
	{
		if (i + 1 <= header_end && data[i] == '\r' && data[i + 1] == '\n')
		{
			size_t line_len = i - line_start;
			if (line_len > max_header_line)
			{
				return (false);
			}
			line_start = i + 2;
			i++; // skip '\n'
		}
	}
	return (true);
}

static ParseStep returnFailure(ParseContext &ctx, ParseStep &step, RequestParseStatus status, std::string msg)
{
	std::ostringstream oss;
	oss << "RequestParser::handleStartLineAndHeaders() → " << toStringStatus(status) << ": " << msg;
	Logger::error(oss.str());

	ctx.phase = ERROR_STATE;
	ctx.request.status = status;
	step.status = status;

	return (step);
}

static ParseStep applyTransferEncoding(ParseContext &ctx, std::string te, size_t consumed_headers, size_t total_len)
{
	ParseStep step;
	step.consumed = consumed_headers;

	if (te != "chunked")
	{
		// TODO: Should return 501 (Not implemented)
		return (returnFailure(ctx, step, PARSE_MALFORMED_REQUEST, "Unsupported Transfer-Encoding"));
	}

	// How many bytes are still available in this buffer after the headers?
	const size_t avail = (total_len > consumed_headers) ? (total_len - consumed_headers) : 0;

	ctx.is_chunked = true;
	ctx.chunk_bytes_remaining = 0;
	ctx.content_length_remaining = 0;
	ctx.total_body_bytes = 0;
	ctx.saw_final_zero = false;
	ctx.last_status = PARSE_INCOMPLETE;
	ctx.read_offset = consumed_headers;
	ctx.phase = READ_CHUNK_SIZE;

	step.status = PARSE_INCOMPLETE;
	step.request_complete = false;
	step.consumed = consumed_headers;
	// If there are bytes after headers, dispatcher can immediately call handleChunkSize()
	step.need_more = (avail == 0);
	return (step);
}

static ParseStep applyContentLength(ParseContext &ctx, std::string cl, size_t consumed_headers, size_t total_len)
{
	ParseStep step;

	// Parse as decimal
	size_t n = 0;
	{
		try
		{
			n = std::stoull(cl);
		}
		catch (const std::exception &)
		{
			return (returnFailure(ctx, step, PARSE_MALFORMED_REQUEST, "Invalid Content-Length"));
		}

		// Enforce body size limit
		if (n > ctx.limits.max_body_size)
		{
			return (returnFailure(ctx, step, PARSE_EXCEED_LIMIT, "Body exceeds max_body_size"));
		}
	}

	// Reset body context
	ctx.is_chunked = false;
	ctx.chunk_bytes_remaining = 0;
	ctx.content_length_remaining = n;
	ctx.total_body_bytes = 0;
	ctx.saw_final_zero = false;
	ctx.read_offset = consumed_headers;
	ctx.last_status = PARSE_INCOMPLETE;

	if (n == 0)
	{
		ctx.phase = COMPLETE;
		ctx.last_status = PARSE_OK;

		step.status = PARSE_OK;
		step.request_complete = true;
		step.need_more = false;
		return (step);
	}

	// How many bytes are still available in this buffer after the headers?
	const size_t avail = (total_len > consumed_headers) ? (total_len - consumed_headers) : 0;

	ctx.phase = READ_BODY_CONTENT_LENGTH;
	step.status = PARSE_INCOMPLETE;
	step.request_complete = false;
	step.consumed = consumed_headers;
	// If there are body bytes in the same buffer, next handler can consume immediately
	step.need_more = (avail == 0);
	return (step);
}

/*
 * Parse start line `/abc?a=a HTTP/1.1`:
 * 	- method: GET
 * 	- path: /abc
 *	- query: a=a
 *
 * Parse heads into map<key, value>.
 */
ParseStep RequestParser::handleStartLineAndHeaders(ParseContext &ctx, const char *data, size_t len) const
{
	ParseStep step;

	Logger::info("RequestParser::handleStartLineAndHeaders()");

	// 1) Find end of header
	size_t header_end = 0;
	if (find_header_end(data, len, header_end) == false)
		return (returnFailure(ctx, step, PARSE_MALFORMED_REQUEST, "Did not find header end"));

	// 2) Check header block size
	if ((header_end + 4) > ctx.limits.max_header_size)
		return (returnFailure(ctx, step, PARSE_EXCEED_LIMIT, "Header length exceeded"));

	// 3) Find end of start-line
	size_t sl_end = (size_t)-1;
	if (find_start_line_end(data, header_end, sl_end) == false)
		return (returnFailure(ctx, step, PARSE_MALFORMED_REQUEST, "No end start-line"));

	// Check start-line size
	if (sl_end > ctx.limits.max_start_line)
		return (returnFailure(ctx, step, PARSE_EXCEED_LIMIT, "Start-line exceeds limits"));

	// 4) Enforce single line header line limit
	if (check_single_header_limit(data, header_end, sl_end, ctx.limits.max_header_line) == false)
		return (returnFailure(ctx, step, PARSE_EXCEED_LIMIT, "Single header line exceeds limits"));

	// 5) Parse Start-Line and Headers
	std::string startLine(data, data + sl_end);
	std::string headerBlock(data + sl_end + 2, data + header_end);

	RequestParseStatus st = PARSE_INCOMPLETE;
	if (!parseStartLine(startLine, ctx.request, st))
		return (returnFailure(ctx, step, PARSE_INCOMPLETE, "Invalid start-line"));
	if (!parseHeaders(headerBlock, ctx.request.headers, st))
		return (returnFailure(ctx, step, PARSE_INCOMPLETE, "Invalid headers"));

	const size_t consumed_headers = header_end + 4;
	step.consumed = consumed_headers;

	// 6) Decide what to do with `body` based on headers
	const std::string te = toLower(searchHeader(ctx.request.headers, "transfer-encoding"));
	const std::string &cl = searchHeader(ctx.request.headers, "content-length");

	// TODO: Treat a message as chunked only if the last coding is chunked.
	if (!te.empty() && !cl.empty())
		return (returnFailure(ctx, step, PARSE_MALFORMED_REQUEST, "Both Transfer-Encoding and Content-Length present"));

	if (!te.empty())
	{
		Logger::info("RequestParser::handleStartLineAndHeaders() → Apply transfer encoding");
		return (applyTransferEncoding(ctx, te, consumed_headers, len));
	}

	if (!cl.empty())
	{
		Logger::info("RequestParser::handleStartLineAndHeaders() → Apply content length");
		return (applyContentLength(ctx, cl, consumed_headers, len));
	}

	// TODO: No Encoding or Content Lenght -> Treat as no body for requests
	step.consumed = consumed_headers;
	ctx.read_offset = consumed_headers;
	ctx.phase = COMPLETE;
	ctx.last_status = PARSE_OK;

	step.status = PARSE_OK;
	step.request_complete = true;
	step.need_more = false;

	return (step);
}
