/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ParseContext.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/01 15:34:19 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/26 11:15:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/ParseContext.hpp"

std::string toStringPhase(ParserPhase phase)
{
	switch (phase)
	{
	case READ_START_LINE:
		return "Read start line";
	case READ_HEADERS:
		return "Read headers";
	case READ_BODY_CONTENT_LENGTH:
		return "Read body (Content-Length)";
	case READ_CHUNK_SIZE:
		return "Read chunk size";
	case READ_CHUNK_DATA:
		return "Read chunk data";
	case COMPLETE:
		return "Complete";
	case ERROR_PHASE:
		return "Error Phase";
	default:
		return "Unknown phase";
	}
}

std::ostream &operator<<(std::ostream &out, const ParseContext ctx)
{
	out << "\n=== ParseContext ==="
		<< "\nPhase: " << toStringPhase(ctx.phase) << "\nLast status: "
		<< toStringStatus(ctx.last_status)

		// Chunk
		<< "\nChunked: " << ctx.is_chunked << "\nChunk bytes remaining: " << ctx.chunk_bytes_remaining
		<< "\nChunk line buffer: " << ctx.chunk_line_buf << "\nTrailer CRLFs: "
		<< ctx.trailer_crlfs

		// Read
		<< "\nRead offset: " << ctx.read_offset << "\nContent length remaining: " << ctx.content_length_remaining
		<< "\nTotal body bytes: " << ctx.total_body_bytes << "\nSaw final zero: "
		<< ctx.saw_final_zero

		// Limits
		// << "\nLimits: "
		// << "\n  Max start line: " << ctx.limits.max_start_line << "\n  Max header line: " <<
		// ctx.limits.max_header_line
		// << "\n  Max header size: " << ctx.limits.max_header_size << "\n  Max body size: "
		// << ctx.limits.max_body_size

		// Request
		// << "\nHttpRequest: " << ctx.request

		<< std::endl;
	return out;
}
