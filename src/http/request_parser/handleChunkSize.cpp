/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleChunkSize.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/01 09:17:41 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/12 20:01:15 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

bool takeChunkLineUntilCRLF(ParseContext &ctx, const char *data, size_t len, size_t &consumed, std::string &out_line)
{
	while (consumed < len)
	{
		ctx.chunk_line_buf.push_back(data[consumed]);
		++consumed;

		if (ctx.chunk_line_buf.size() > ctx.limits.max_header_line)
			return (false);

		size_t n = ctx.chunk_line_buf.size();
		if (n >= 2 && ctx.chunk_line_buf[n - 2] == '\r' && ctx.chunk_line_buf[n - 1] == '\n')
		{
			out_line.assign(ctx.chunk_line_buf.c_str(), n - 2); // strip CRLF
			ctx.chunk_line_buf.clear();
			return (true);
		}
	}
	return (false);
}

inline bool isHex(char ch)
{
	return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}
inline unsigned hexVal(char ch)
{
	if (ch >= '0' && ch <= '9')
		return (ch - '0');
	if (ch >= 'a' && ch <= 'f')
		return (10u + (ch - 'a'));
	return (10u + (ch - 'A'));
}

bool parseChunkSizeHex(const std::string &line, size_t &out)
{
	const std::string::size_type semi = line.find(';');
	const std::string hex = (semi == std::string::npos) ? line : line.substr(0, semi);

	if (hex.empty())
		return (false);

	// TODO: trim spaces around hex
	size_t b = 0, e = hex.size();

	size_t val = 0;
	for (size_t i = b; i < e; ++i)
	{
		char ch = hex[i];
		if (!isHex(ch))
			return false;
		const size_t prev = val;
		val = (val << 4) + hexVal(ch);
		if (val < prev)
			return (false); // Overflow
	}
	out = val;
	return (true);
}

bool exceedsBodyLimit(const ParseContext &ctx, size_t next_chunk)
{
	if (next_chunk == 0)
		return (false); // The terminal chunk doesn't add body bytes
	return (ctx.total_body_bytes > ctx.limits.max_body_size - next_chunk);
}

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
 * Parses exactly one chunk-size line `<hex>[;ext]\r\n` to determine chunck size
 * Size of chunk is stored in `ctx.chunk_bytes_remaining`
 */
ParseStep RequestParser::handleChunkSize(ParseContext &ctx, const char *data, size_t len) const
{
	ParseStep step;

	// 1) Try to complete the "<hex>[;ext]\r\n" line
	std::string line;
	size_t consumed_now = ctx.read_offset;
	const bool got_line = takeChunkLineUntilCRLF(ctx, data, len, consumed_now, line);

	step.consumed = line.size() + 2;
	ctx.read_offset = consumed_now;

	if (!got_line)
	{
		// No full line yet
		if (ctx.chunk_line_buf.size() > ctx.limits.max_header_line)
			return (returnError(ctx, PARSE_EXCEED_BODY_LIMIT, "No full line found"));
		step.status = PARSE_INCOMPLETE;
		step.need_more = true;
		return (step);
	}

	// 2) Parse hex size (ignore extensions)
	size_t size_val = 0;
	if (!parseChunkSizeHex(line, size_val))
	{
		return (returnError(ctx, PARSE_MALFORMED_REQUEST, "Failed to parse chunked size as hex"));
	}

	// 3) Enforce body size limit (future payload + already copied)
	if (exceedsBodyLimit(ctx, size_val))
	{
		return (returnError(ctx, PARSE_EXCEED_BODY_LIMIT, "Body size limits exceeded"));
	}

	// 4) Transition: zero-sized final chunk vs normal chunk
	if (size_val == 0)
	{
		ctx.saw_final_zero = true;
		ctx.chunk_bytes_remaining = 0;
		ctx.trailer_crlfs = 0;		 // Use in handleChunkData() to detect CRLFCRLF
		ctx.phase = READ_CHUNK_DATA; // Next: consume trailers/terminator
	}
	else
	{
		Logger::debug("RequestParser::handleChunkSize() → Size for chunk: " + std::to_string(size_val));
		ctx.chunk_bytes_remaining = size_val;
		ctx.phase = READ_CHUNK_DATA; // Next: read exactly size_val bytes + CRLF
	}

	step.status = PARSE_INCOMPLETE;
	step.need_more = true;

	return (step);
}
