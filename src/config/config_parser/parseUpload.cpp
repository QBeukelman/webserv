/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseUpload.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:25:07 by hein          #+#    #+#                 */
/*   Updated: 2025/09/30 12:08:11 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

#include <filesystem>
#include <unistd.h>

static void validateUploadPath(std::string &token, TokenStream &tokenStream)
{
	if (!std::filesystem::exists(token))
	{
		tokenStream.throwError("The given Upload Store does not exist");
	}
	if (!std::filesystem::is_directory(token))
	{
		tokenStream.throwError("The given Upload Store is not a Directory");
	}
	if (access(token.c_str(), X_OK | W_OK) != 0)
	{
		tokenStream.throwError("The given Upload Store does not permit the right Permissions");
	}
}

void ConfigParser::parseUpload(Location &location, TokenStream &tokenStream)
{
	if (location.hasDirective(UPLOAD))
	{
		tokenStream.throwError("Duplicate Uploads in a Location are not allowed");
	}

	tokenStream.removeValidSemicolon();

	tokenStream.validateExpectedArguments(1);

	std::string token = tokenStream.next();

	validateUploadPath(token, tokenStream);

	location.addUploadDirectory(token);

	location.markDirective(UPLOAD);
}