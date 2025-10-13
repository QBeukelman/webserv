/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseRedirect.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:24:24 by hein          #+#    #+#                 */
/*   Updated: 2025/10/10 11:00:12 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

static HttpStatus validateReturnCode(const std::string &token, TokenStream &tokenStream)
{
	if (!isDigitString(token))
	{
		tokenStream.throwError("Found non numeric value in the Return code");
	}

	HttpStatus code = toHttpStatus(std::stoi(token));

	if (code != 301 && code != 302 && code != 303 && code != 304 && code != 307)
	{
		tokenStream.throwError("Return code is not supported. Expected [ 301 - 302 - 303 - 304 - 307 ]");
	}
	return (code);
}

static void validateArgumentCount(const std::size_t &argumentCount, HttpStatus &code, TokenStream &tokenStream)
{
	if (code < 400)
	{
		if (argumentCount != 2)
		{
			tokenStream.throwError("Unexpected amount of arguments. Expected [ 2 ]");
		}
		return;
	}
	if (code > 399)
	{
		if (argumentCount != 1)
		{
			tokenStream.throwError("Unexpected amount of arguments. Expected [ 1 ]");
		}
	}
}

static void validateRedirect(const std::string &token, TokenStream &tokenStream)
{
	if (!(token.size() >= 1 && token.compare(0, 1, "/") == 0) &&
		!(token.size() >= 2 && token.compare(0, 2, "./") == 0) &&
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

	Redirect redirect;

	tokenStream.removeValidSemicolon();

	const std::size_t argumentCount = tokenStream.validateMinimumArguments(1);

	std::string token = tokenStream.next();

	redirect.httpStatus = validateReturnCode(token, tokenStream);

	validateArgumentCount(argumentCount, redirect.httpStatus, tokenStream);

	if (redirect.httpStatus < 400)
	{
		token = tokenStream.next();

		validateRedirect(token, tokenStream);

		redirect.path = token;
	}

	location.setRedirect(redirect);

	location.markDirective(RETURN);
}
