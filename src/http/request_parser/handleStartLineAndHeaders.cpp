/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleStartLineAndHeaders.cpp                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/28 20:39:56 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/28 22:06:40 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

static bool find_header_end(const char *data, size_t len, size_t &pos)
{
	if (len < 4)
		return (false);
	for (size_t i = 0; i + 3 < len; ++i)
	{
		if (data[i] == '\r' && data[i + 1] == '\n' && data[i + 2] == '\r' && data[i + 3] == '\n')
		{
			pos = i;
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
		// TODO: No end start-line
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

ParseStep RequestParser::handleStartLineAndHeaders(ParseContext &context, const char *data, size_t len) const
{
	ParseStep step;

	// 1. Find end of header
	size_t header_end = 0;
	if (find_header_end(data, len, header_end) == false)
	{
		// TODO: Did not find header end
	}

	// 2. Check header block size
	if ((header_end + 4) > context.limits.max_header_size)
	{
		// TODO: Header length exceeded
	}

	// 3. Find end of start-line
	size_t sl_end = (size_t)-1;
	if (find_start_line_end(data, header_end, sl_end) == false)
	{
		// TODO: No end start-line
	}

	// Check start-line size
	if (sl_end > context.limits.max_start_line)
	{
		// TODO: Start-line exceeds limits
	}

	// 4. Enforce single line header line limit
	if (check_single_header_limit(data, header_end, sl_end, context.limits.max_header_line) == false)
	{
		// TODO: Single header line exceeds limits
	}

	// 5. Parse Start-Line and Headers
	std::string startLine(data, data + sl_end);
	std::string headerBlock(data + sl_end + 2, data + header_end);
	RequestParseStatus st = PARSE_INCOMPLETE;
	if (parseStartLine(startLine, context.request, st) == false)
	{
		// TODO: Start-line error
	}
	if (parseHeaders(headerBlock, context.request.headers, st) == false)
	{
		// TODO: Header error
	}

	// Look for:
	// "Transfer-Encoding"
	// "Content-Length"

	// TODO: Fill ParseStep
	return (ParseStep());
}
