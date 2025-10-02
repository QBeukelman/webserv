/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseAllowMethods.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/02 13:49:23 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

void ConfigParser::parseAllowMethod(Location &location, TokenStream &tokenStream)
{
	if (location.hasDirective(ALLOWED))
	{
		tokenStream.throwError("Duplicate allow_methods Directive is not allowed");
	}

	tokenStream.removeValidSemicolon();

	const std::size_t argumentCount = tokenStream.validateMinimumArguments(1);

	if (argumentCount > 3)
	{
		tokenStream.throwError("The amount of arguments exceeds the maximum amount of [ 3 ]");
	}

	for (std::size_t i = 0; i < argumentCount; ++i)
	{
		std::string token = tokenStream.next();

		toLower(token);

		if (token != "get" && token != "post" && token != "delete")
		{
			tokenStream.throwError("Found an unsupported Method [ " + token + " ]");
		}

		location.allowed_methods.insert(toMethod(token));
	}

	location.markDirective(ALLOWED);
}
