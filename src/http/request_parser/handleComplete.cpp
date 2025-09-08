/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleComplete.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/01 17:31:00 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/01 20:23:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

ParseStep RequestParser::handleComplete(ParseContext &ctx) const
{
	ParseStep step;

	// Do not consume anything
	step.consumed = 0;
	step.request_complete = true;
	step.need_more = false;

	// Set step status
	if (ctx.last_status == PARSE_OK || ctx.last_status == PARSE_INCOMPLETE)
		step.status = PARSE_OK;
	else
		step.status = ctx.last_status;

	// Update context
	ctx.phase = COMPLETE;
	ctx.last_status = step.status;
	ctx.request.status = PARSE_OK;

	return (step);
}
