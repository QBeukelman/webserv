/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleBodyContentLength.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/30 15:25:36 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/01 09:12:30 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

ParseStep RequestParser::handleBodyContentLength(ParseContext &ctx, const char *data, size_t len) const
{
	ParseStep step;

	// Already complete?
	if (ctx.content_length_remaining == 0)
	{
		// TODO: Is already finished
		return (step);
	}

	// Nothing to read
	if (len == 0)
	{
		// Keep step refaults
		return (step);
	}

	// Copy only what fits in the remaining body length
	size_t to_copy = len;
	if (to_copy > ctx.content_length_remaining)
		to_copy = ctx.content_length_remaining;

	// Enforce body limits (total bytes accross calls)
	if (ctx.total_body_bytes + to_copy > ctx.limits.max_body_size)
	{
		// TODO: ctx and step duplicate status
		ctx.phase = ERROR_STATE;
		ctx.last_status = PARSE_EXCEED_LIMIT;
		step.status = PARSE_EXCEED_LIMIT;
		step.need_more = false;
		return (step);
	}

	// Append body slice
	ctx.request.body.append(data, to_copy);
	ctx.total_body_bytes += to_copy;
	ctx.content_length_remaining -= to_copy;

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
	step.need_more = false;
	step.request_complete = true;
	step.status = PARSE_OK;
	return (step);
}
