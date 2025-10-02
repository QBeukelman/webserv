/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseAutoIndex.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:22:21 by hein          #+#    #+#                 */
/*   Updated: 2025/10/01 10:43:06 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

static bool validateAutoindexArgument(const std::string &token, TokenStream &tokenStream)
{
	if (token == "on")
	{
		return (true);
	}
	if (token == "off")
	{
		return (false);
	}
	tokenStream.throwError("Found invalid argument for Autoindex. Expected [ on ] or [ off ]");
	return (false);
}

void ConfigParser::parseAutoIndex(Location &location, TokenStream &tokenStream)
{
	if (location.hasDirective(AUTOINDEX))
	{
		tokenStream.throwError("Duplicate Autoindex directive is not allowed");
	}

	bool autoindex = false;

	tokenStream.removeValidSemicolon();

	tokenStream.validateExpectedArguments(1);

	std::string token = tokenStream.next();

	toLower(token);

	autoindex = validateAutoindexArgument(token, tokenStream);

	location.setAutoindex(autoindex);

	location.markDirective(AUTOINDEX);
}