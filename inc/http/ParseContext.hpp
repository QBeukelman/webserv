/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ParseContext.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/28 14:56:11 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/28 20:34:14 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSECONTEXT_HPP
#define PARSECONTEXT_HPP

#include "http/HttpRequestLimits.hpp"
#include "http/Request.hpp"
#include "http/RequestParser.hpp"

enum ParserPhase
{
	READ_START_LINE,
	READ_HEADERS,
	READ_BODY_CONTENT_LENGTH,
	READ_CHUNK_SIZE,
	READ_CHUNK_DATA,
	COMPLETE,
	ERROR_STATE
};

/*
 * This structure represents the *current parsing state* of a single client
 * connection. It is owned by the event loop and passed to the RequestParser.
 *
 * Usage:
 *   - The parser must be incremental: consume available bytes, update state, and ask for more if needed.
 *   - Initialize a fresh ParseContext for each new request on a connection.
 *   - The event loop owns this object and passes it into parser.step().
 *   - parser.step() consumes bytes, mutates fields, and updates phase.
 *   - When phase == COMPLETE, the request is ready to be executed.
 *   - Reset (or reinit) ParseContext after handing off a complete request,
 *     so pipelined requests on the same connection can reuse it.
 */
struct ParseContext
{
	ParserPhase phase; // current phase in state machine
	HttpRequest request;
	HttpRequestLimits limits;

	size_t content_length_remaining;

	size_t chunk_bytes_remaining;
	bool saw_final_zero;

	size_t total_body_bytes;
	RequestParseStatus last_status;

	ParseContext()
		: phase(READ_START_LINE), content_length_remaining(0), chunk_bytes_remaining(0), saw_final_zero(false),
		  total_body_bytes(0), last_status(PARSE_INCOMPLETE)
	{
	}
};

#endif
