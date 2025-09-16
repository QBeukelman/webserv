/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseRoot.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:24:43 by hein          #+#    #+#                 */
/*   Updated: 2025/09/16 13:56:45 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

#include <filesystem>
#include <unistd.h>

static void validateRootPath(std::string &token, TokenStream &tokenStream)
{
	if (!std::filesystem::exists(token))
	{
		tokenStream.throwError("The given Root path does not exist");
	}
	if (!std::filesystem::is_directory(token))
	{
		tokenStream.throwError("The given Root path is not a Directory");
	}
	if (access(token.c_str(), R_OK | X_OK) != 0)
	{
		tokenStream.throwError("The given Root path does not permit the right Permissions");
	}
}

void ConfigParser::parseRoot(Server &server, TokenStream &tokenStream)
{
	if (server.hasDirective(ROOT))
	{
		tokenStream.throwError("Diplicate Root directives are not allowed");
	}

	tokenStream.removeValidSemicolon();

	tokenStream.validateExpectedArguments(1);

	std::string token = tokenStream.next();

	validateRootPath(token, tokenStream);

	server.setRoot(token);

	server.markDirective(ROOT);
}
