/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parserHeaders.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 16:37:38 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/01 20:13:12 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

/*
 * Parse raw header block into key/value map.
 *
 * Notes:
 * 	- Will update status on error.
 */
bool RequestParser::parseHeaders(const std::string &headerBlock, std::map<std::string, std::string> &headers,
								 RequestParseStatus &status) const
{
	// 1) Split into raw lines on CRLF
	std::vector<std::string> lines;
	lines.reserve(16);

	std::string::size_type start = 0;
	while (start < headerBlock.size())
	{
		std::string::size_type end = headerBlock.find("\r\n", start);
		if (end == std::string::npos)
			break;
		lines.push_back(headerBlock.substr(start, end - start));
		start = end + 2;
	}
	if (start < headerBlock.size())
		lines.push_back(headerBlock.substr(start));

	// Allow empty header block
	if (lines.empty())
		return (true);

	// 2) Parse Name : Value
	for (size_t i = 0; i < lines.size(); ++i)
	{
		const std::string &line = lines[i];
		if (line.empty())
		{
			status = PARSE_INCOMPLETE;
			return (false);
		}

		std::string::size_type colon = line.find(':');
		if (colon == std::string::npos || colon == 0)
		{
			// no colon or empty name
			status = PARSE_INCOMPLETE;
			return (false);
		}

		std::string name = toLower(line.substr(0, colon));
		std::string value = trim(line.substr(colon + 1));

		headers[name] = value;
	}
	return (true);
}
