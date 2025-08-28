/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parserUtils.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/28 11:14:05 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/28 22:15:27 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

std::string RequestParser::toLower(const std::string &s) const
{
	std::string out;
	out.reserve(s.size());
	for (size_t i = 0; i < s.size(); ++i)
		out.push_back(std::tolower(static_cast<unsigned char>(s[i])));
	return (out);
}

std::string RequestParser::trim(const std::string &s) const
{
	std::string::size_type start = s.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return ("");
	std::string::size_type end = s.find_last_not_of(" \t\r\n");
	return (s.substr(start, end - start + 1));
}
