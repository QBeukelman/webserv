/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleChunkData.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/01 16:12:29 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/26 11:15:56 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

inline ParseStep returnError(ParseContext &ctx, RequestParseStatus status, std::string msg)
{
	ParseStep s;
	ctx.phase = ERROR_PHASE;
	s.status = status;
	s.need_more = false;

	std::ostringstream oss;
	oss << "RequestParser::handleChunkSize() → " << toStringStatus(status) << ": " << msg;
	Logger::error(oss.str());

	return (s);
}

/*
 * Handle chunck data: Parse exacty context.chunk_bytes_remaining from header.
 */
ParseStep RequestParser::handleChunkData(ParseContext &ctx, const char *data, size_t len) const
{
	ParseStep step;

	// 1) Consume as many bytes as possible from chunk
	const size_t available = len - ctx.read_offset;
	const size_t to_copy = std::min(ctx.chunk_bytes_remaining, available);

	if (to_copy > 0)
	{
		ctx.request.body.append(data + ctx.read_offset, to_copy);
		ctx.read_offset += to_copy;
		ctx.chunk_bytes_remaining -= to_copy;
		step.consumed += to_copy;
		ctx.total_body_bytes += to_copy;
	}

	// 2) If waiting for more data
	if (ctx.chunk_bytes_remaining > 0)
	{
		step.status = PARSE_INCOMPLETE;
		step.need_more = true;
		return (step);
	}

	// 3) Chuck done -> require CRLF after it
	if (len - ctx.read_offset < 2)
	{
		step.status = PARSE_INCOMPLETE;
		step.need_more = true;
		return (step);
	}

	if (data[ctx.read_offset] != '\r' || data[ctx.read_offset + 1] != '\n')
	{
		return returnError(ctx, PARSE_MALFORMED_REQUEST, "Missing CRLF after chunk data");
	}
	ctx.read_offset += 2;
	step.consumed += 2;

	// 4) Handle after chunk
	if (ctx.saw_final_zero)
	{
		// Final chunk -> trailers or end
		ctx.phase = COMPLETE;
		step.status = PARSE_OK;
		step.request_complete = true;
		step.need_more = false;
	}
	else
	{
		// Expect next chunk header
		ctx.phase = READ_CHUNK_SIZE;
		step.status = PARSE_INCOMPLETE;
		step.need_more = true;
	}

	return (step);
}
