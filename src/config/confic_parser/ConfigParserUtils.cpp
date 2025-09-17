/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParserUtils.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/15 10:02:41 by hein          #+#    #+#                 */
/*   Updated: 2025/09/15 15:04:44 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/config_parser/ConfigParser.hpp"

bool isalnumString(const std::string &s)
{
	for (char c : s)
	{
		if (!std::isalnum(static_cast<unsigned char>(c)))
			return (false);
	}
	return (true);
}

bool isDigitString(const std::string &s)
{
	for (char c : s)
	{
		if (!std::isdigit(static_cast<unsigned char>(c)))
			return (false);
	}
	return (true);
}

void toLower(std::string &s)
{
	for (char &c : s)
	{
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	}
}