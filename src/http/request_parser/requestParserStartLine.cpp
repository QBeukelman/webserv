/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   requestParserStartLine.cpp                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/27 08:43:28 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/28 08:33:33 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

bool isValidTarget(const std::string &s)
{
	if (s.empty())
		return (false);

	for (size_t i = 0; i < s.size(); ++i)
	{
		unsigned char c = s[i];

		// disallow CTL and space
		if (c <= 0x1F || c == 0x7F || c == ' ')
			return false;

		// percent-encoding check
		// e.g. GET /docs/My%20File.pdf HTTP/1.1
		if (c == '%')
		{
			if (i + 2 >= s.size())
				return false;
			if (!std::isxdigit(s[i + 1]) || !std::isxdigit(s[i + 2]))
				return false;
			i += 2;
			continue;
		}

		if (std::isalnum(c))
			continue;

		switch (c)
		{
		case '-':
		case '.':
		case '_':
		case '~':
		case '!':
		case '$':
		case '&':
		case '\'':
		case '(':
		case ')':
		case '*':
		case '+':
		case ',':
		case ';':
		case '=':
		case ':':
		case '@':
		case '/':
		case '?':
			continue;
		default:
			return false;
		}
	}
	return true;
}

bool isValidStartLine(const std::vector<std::string> &tokens)
{
	std::ostringstream oss;

	if (tokens.size() != 3)
	{
		oss << "RequestParser::parseStartLine() -> Invalid token count: " << tokens.size();
		Logger::error(oss.str());
		return (false);
	}

	// Method
	const std::string &method = tokens[0];
	if (isSupported(method) == false)
	{
		oss << "RequestParser::parseStartLine() -> Unsupported method: " << method;
		Logger::error(oss.str());
		return (false);
	}

	// Request target
	const std::string &target = tokens[1];
	if (isValidTarget(tokens[1]) == false)
	{
		oss << "RequestParser::parseStartLine() -> Unsupported chars in request target " << target;
		Logger::error(oss.str());
		return (false);
	}
	if (target[0] != '/')
	{
		Logger::error("RequestParser::parseStartLine() -> Request target must start with '/'");
		return (false);
	}

	// HTTP version
	const std::string &version = tokens[2];
	if (version != "HTTP/1.1")
	{
		oss << "RequestParser::parseStartLine() -> Unsupported HTTP version (HTTP/1.1) " << version;
		Logger::error(oss.str());
		return (false);
	}
	return (true);
}

void splitTarget(const std::string &target, std::string &path, std::string &query)
{
	const std::string::size_type q = target.find("?");
	if (q == std::string::npos)
	{
		path = target;
		query.clear();
	}
	else
	{
		path = target.substr(0, q);
		query = target.substr(q + 1);
	}
}

bool RequestParser::parseStartLine(const std::string &startLine, HttpRequest &out, RequestParseStatus &status) const
{
	std::istringstream iss(startLine);
	std::vector<std::string> tokens;
	std::string word;

	while (iss >> word)
	{
		tokens.push_back(word);
	}

	if (isValidStartLine(tokens) == false)
	{
		status = PARSE_MALFORMED_REQUEST;
		out.method = HttpMethod::UNKNOWN;
		return (false);
	}

	out.method = toMethod(tokens[0]);
	out.target = tokens[1];
	out.version = tokens[2];

	splitTarget(out.target, out.path, out.query);

	return (true);
}
