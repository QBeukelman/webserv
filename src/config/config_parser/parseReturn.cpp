/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseReturn.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:24:24 by hein          #+#    #+#                 */
/*   Updated: 2025/10/01 10:40:32 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

static unsigned short validateReturnCode(const std::string &token, TokenStream &tokenStream)
{
	if (!isDigitString(token))
	{
		tokenStream.throwError("Found non numeric value in the Return code");
	}

	unsigned short code = std::stoi(token);

	if (code < 300 && code > 500)
	{
		tokenStream.throwError("Return code is outside of supported range [ 300 - 500 ]");
	}
	return (code);
}

static void validateArgumentCount(const std::size_t &argumentCount, unsigned short &code, TokenStream &tokenStream)
{
	if (code < 400 && argumentCount != 2)
	{
		tokenStream.throwError("Unexpected amount of arguments. Expected [ 2 ]");
	}
	else if (argumentCount != 1)
	{
		tokenStream.throwError("Unexpected amount of arguments. Expected [ 1 ]");
	}
}

static void validateRedirection(const std::string &token, TokenStream &tokenStream)
{
	if (!(token.size() >= 1 && token.compare(0, 1, "/") == 0) &&
		!(token.size() >= 7 && token.compare(0, 7, "http://") == 0) &&
		!(token.size() >= 8 && token.compare(0, 8, "https://") == 0))
	{
		tokenStream.throwError("Expected the Return path to start with [ / ] or [ http:// ] or [ https:// ]");
	}
}

void ConfigParser::parseReturn(Location &location, TokenStream &tokenStream)
{
	if (location.hasDirective(RETURN))
	{
		tokenStream.throwError("Duplicate Return directive is not allowed");
	}
	if (location.hasDirective(ALLOWED | UPLOAD | AUTOINDEX | ROOT | INDEX))
	{
		tokenStream.throwError("Return directive cannot be combined with any other location directives");
	}

	Redirection redirect;

	tokenStream.removeValidSemicolon();

	const std::size_t argumentCount = tokenStream.validateMinimumArguments(1);

	std::string token = tokenStream.next();

	redirect.code = validateReturnCode(token, tokenStream);

	validateArgumentCount(argumentCount, redirect.code, tokenStream);

	if (redirect.code < 400)
	{
		token = tokenStream.next();

		validateRedirection(token, tokenStream);

		redirect.path = token;
	}

	location.setReturn(redirect);

	location.markDirective(RETURN);
}