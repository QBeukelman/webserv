/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseErrorPage.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:22:39 by hein          #+#    #+#                 */
/*   Updated: 2025/10/03 09:31:50 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

#include <filesystem>
#include <unistd.h>

static HttpStatus validateErrorCode(std::string &token, TokenStream &tokenStream)
{
	if (!isDigitString(token))
	{
		tokenStream.throwError("Error Code contains non numeric characters");
	}

	unsigned short code = std::stoi(token);

	if (code < 300 || code > 599)
	{
		tokenStream.throwError("Error Code is out of standard HTML Error range [ 300 - 599 ]");
	}
	return (toHttpStatus(code));
}

static void validateErrorPage(std::string &token, TokenStream &tokenStream)
{
	if (token.front() != '/' && (token.compare(0, 2, "./") != 0))
	{
		tokenStream.throwError("Error Page path must start with either [ / ] or [ ./ ]");
	}
	if (!std::filesystem::exists(token))
	{
		tokenStream.throwError("Error Page does not exist");
	}
	if (!std::filesystem::is_regular_file(token))
	{
		tokenStream.throwError("Error Page is not a Regular file");
	}
	if (access(token.c_str(), R_OK) != 0)
	{
		tokenStream.throwError("Error Page is not readable (Permission denied)");
	}
}

void ConfigParser::parseErrorPage(Server &server, TokenStream &tokenStream)
{
	tokenStream.removeValidSemicolon();

	tokenStream.validateExpectedArguments(2);

	ErrorPage newErrorPage;

	std::string token = tokenStream.next();

	newErrorPage.httpStatus = validateErrorCode(token, tokenStream);

	token = tokenStream.next();

	validateErrorPage(token, tokenStream);

	newErrorPage.path = token;

	if (!server.addErrorPage(newErrorPage))
	{
		tokenStream.throwError("Duplicate Error Codes are not allowed");
	}

	server.markDirective(ERRORPAGE);
}