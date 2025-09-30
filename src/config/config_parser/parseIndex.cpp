/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseIndex.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:07:10 by hein          #+#    #+#                 */
/*   Updated: 2025/09/19 09:39:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

static void catchWrongInput(std::string &token, TokenStream &tokenStream)
{
	if (token.find('.') == std::string::npos)
	{
		tokenStream.throwError("Expected at least one dot [ . ] in filename but found none");
	}
	if (token.front() == '.')
	{
		tokenStream.throwError(
			"Found a dot [ . ] at the beginning of the index filename. Hidden files are not supported");
	}
	if (token.find("..") != std::string::npos)
	{
		tokenStream.throwError("Found consecutive dots [ .. ] in the index filename");
	}
	if (token.find('/') != std::string::npos)
	{
		tokenStream.throwError("Forward Slash [ / ] is not allowed in the index filename");
	}
}

static void validateIndexFile(std::string &token, TokenStream &tokenStream)
{
	catchWrongInput(token, tokenStream);

	for (char c : token)
	{
		if (!std::isalnum(static_cast<unsigned char>(c)) && (c != '.' && c != '-' && c != '_'))
		{
			tokenStream.throwError("Found an forbidden character [ " + std::string(1, c) + " ] in the index filename");
		}
	}
}

template <typename T> void ConfigParser::parseIndex(T &container, TokenStream &tokenStream)
{
	tokenStream.removeValidSemicolon();

	std::size_t argumentCount = tokenStream.validateMinimumArguments(1);

	for (std::size_t i = 0; i < argumentCount; ++i)
	{
		std::string token = tokenStream.next();

		validateIndexFile(token, tokenStream);

		if (!container.addIndexFile(token))
		{
			tokenStream.throwError("Duplicate Index Files are not allowed");
		}
	}

	container.markDirective(INDEX);
}

template void ConfigParser::parseIndex<Server>(Server &, TokenStream &);
template void ConfigParser::parseIndex<Location>(Location &, TokenStream &);