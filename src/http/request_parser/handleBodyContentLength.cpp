/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleBodyContentLength.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/30 15:25:36 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/05 15:59:39 by quentinbeuk   ########   odam.nl         */
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
		step.consumed = 0; // nothing new consumed in this call
		return (step);
	}

	// Bytes available in the current window after the headers (and previous reads)
	const size_t available = len - ctx.read_offset;
	if (available == 0)
	{
		step.status = PARSE_INCOMPLETE;
		step.need_more = true;
		step.request_complete = false;
		step.consumed = 0;
		return (step);
	}

	// Copy only what fits in the remaining body length
	size_t to_copy = std::min(available, ctx.content_length_remaining);

	// Enforce max body size across calls
	if (ctx.total_body_bytes + to_copy > ctx.limits.max_body_size)
	{
		ctx.phase = ERROR_STATE;
		ctx.last_status = PARSE_EXCEED_LIMIT;
		step.status = PARSE_EXCEED_LIMIT;
		step.need_more = false;
		return (step);
	}

	// Append the slice starting at read_offset
	ctx.request.body.append(data + ctx.read_offset, to_copy);
	ctx.total_body_bytes += to_copy;
	ctx.content_length_remaining -= to_copy;
	ctx.read_offset += to_copy;
	step.consumed = to_copy;

	// Decide next step
	if (ctx.content_length_remaining > 0)
	{
		// Still need more bytes -> remain in READ_BODY_CONTENT_LENGTH
		step.status = PARSE_INCOMPLETE;
		step.need_more = true;
		step.request_complete = false;
		return (step);
	}

	// Body complete
	ctx.phase = COMPLETE;
	ctx.request.status = PARSE_OK;
	step.need_more = false;
	step.request_complete = true;
	step.status = PARSE_OK;
	return (step);
}
