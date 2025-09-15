/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseName.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:24:03 by hein          #+#    #+#                 */
/*   Updated: 2025/09/15 13:02:53 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

static void validateServername(const std::string &token, TokenStream &tokenStream)
{
	for (char c : token)
	{
		if (!std::isalnum(static_cast<unsigned char>(c)) && (c != '.' && c != '-'))
		{
			tokenStream.throwError("Found invalid character in server name [ " + std::string(1, c) + " ]");
		}
	}
	if (token.front() == '.' || token.back() == '.')
	{
		tokenStream.throwError("Server Name can not start or end with a dot [ . ]");
	}
	else if (token.find("..") != std::string::npos)
	{
		tokenStream.throwError("Found consecutive dots [ .. ]");
	}
	else if (token.length() > 255)
	{
		tokenStream.throwError("Server name exceded the max length of 255 characters");
	}
}

static void validateLabels(const std::string &token, TokenStream &tokenStream)
{
	std::stringstream ss(token);
	std::string label;

	while (std::getline(ss, label, '.'))
	{

		if (token.front() == '-' || token.back() == '-')
		{
			tokenStream.throwError("Leading or Trailing hyphen [ - ] in the label are not allowed");
		}
		else if (token.length() > 63)
		{
			tokenStream.throwError("Label size exceded the max length of 63 characters");
		}
	}
}

void ConfigParser::parseName(Server &server, TokenStream &tokenStream)
{
	std::size_t argumentCount = tokenStream.validateMinimumArguments(1);

	tokenStream.removeValidSemicolon();

	for (std::size_t i = 0; i < argumentCount; ++i)
	{
		std::string token = tokenStream.next();

		toLower(token);

		validateServername(token, tokenStream);

		validateLabels(token, tokenStream);

		if (!server.setName(token))
		{
			tokenStream.throwError("Duplicate server names are not allowed");
		}
	}
}