/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleBodyContentLength.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/30 15:25:36 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 16:08:32 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

ParseStep RequestParser::handleBodyContentLength(ParseContext &ctx, const char *data, size_t len) const
{
	ParseStep step;

	// Already complete?
	if (ctx.content_length_remaining == 0)
	{
		ctx.phase = COMPLETE;
		ctx.last_status = PARSE_OK;
		step.status = PARSE_OK;
		step.need_more = false;
		step.request_complete = true;
		step.consumed = 0;
		return (step);
	}

	// No bytes in this window
	if (len == 0)
	{
		step.status = PARSE_INCOMPLETE;
		step.need_more = true;
		step.request_complete = false;
		step.consumed = 0;
		return (step);
	}

	// Copy only what fits
	const size_t available = len - ctx.read_offset;
	size_t to_copy = std::min(available, ctx.content_length_remaining);

	// Max body size
	if (ctx.total_body_bytes + to_copy > ctx.limits.max_body_size)
	{
		ctx.phase = ERROR_PHASE;
		ctx.last_status = PARSE_EXCEED_BODY_LIMIT;
		step.status = PARSE_EXCEED_BODY_LIMIT;
		step.need_more = false;
		step.request_complete = false;
		Logger::error("handleBodyContentLength() → Max body size");
		return (step);
	}

	// Append from beginning of window
	ctx.request.body.append(data + ctx.read_offset, to_copy);
	ctx.total_body_bytes += to_copy;
	ctx.content_length_remaining -= to_copy;
	ctx.read_offset += to_copy;

	step.consumed = to_copy;
	step.need_more = (ctx.content_length_remaining > 0);
	step.request_complete = (ctx.content_length_remaining == 0);
	step.status = step.request_complete ? PARSE_OK : PARSE_INCOMPLETE;

	// Mismatched Content-Length and Body len
	const std::string content_length_header = ctx.request.searchHeader("Content-Length");
	if (content_length_header != "")
	{
		int content_length;
		try
		{
			content_length = stoi(content_length_header);
			if (ctx.request.body.size() != content_length)
			{
				Logger::info("RequestParser::handleBodyContentLength() → Mismatch body len");
				ctx.mismatch_body_len = true;
			}
			else
			{
				Logger::info("RequestParser::handleBodyContentLength() → Matched body len");
				ctx.mismatch_body_len = false;
			}
		}
		catch (...)
		{
		}
	}

	// Complete
	if (step.request_complete)
	{
		ctx.phase = COMPLETE;
		ctx.last_status = PARSE_OK;
	}
	return (step);
}
