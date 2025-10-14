/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ParseContext.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/28 14:56:11 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/14 01:58:22 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSECONTEXT_HPP
#define PARSECONTEXT_HPP

#include "HttpRequest.hpp"
#include "HttpRequestLimits.hpp"

enum ParserPhase
{
	READ_START_LINE,
	READ_HEADERS,
	READ_BODY_CONTENT_LENGTH,
	READ_CHUNK_SIZE,
	READ_CHUNK_DATA,
	READ_MULTIPART,
	COMPLETE,
	ERROR_PHASE
};

std::string toStringPhase(ParserPhase);

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

	// Chunk
	bool is_chunked;
	size_t chunk_bytes_remaining;
	std::string chunk_line_buf; // accumulates "<size>[;ext]\r\n"
	size_t trailer_crlfs;

	// Read
	size_t read_offset; // bytes consumed from the current input window
	size_t content_length_remaining;
	size_t total_body_bytes;

	bool saw_final_zero;
	bool mismatch_body_len;

	// TODO: ctx and step duplicate status
	RequestParseStatus last_status;

	ParseContext()
		: phase(READ_START_LINE), limits(HttpRequestLimits{}), is_chunked(false), chunk_bytes_remaining(0),
		  trailer_crlfs(0), read_offset(0), content_length_remaining(0), total_body_bytes(0), saw_final_zero(false),
		  mismatch_body_len(false), last_status(PARSE_INCOMPLETE)
	{
	}
};

std::ostream &operator<<(std::ostream &out, ParseContext ctx);

#endif
