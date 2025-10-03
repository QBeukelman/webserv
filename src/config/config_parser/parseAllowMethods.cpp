/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseAllowMethods.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/03 10:56:47 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

static bool isAllCaps(const std::string &method)
{
	for (const auto &c : method)
	{
		if (c <= 'A' || c >= 'Z')
			return (false);
	}
	return (true);
}

void ConfigParser::parseAllowMethod(Location &location, TokenStream &tokenStream)
{
	if (location.hasDirective(ALLOWED))
		tokenStream.throwError("Duplicate allow_methods Directive is not allowed");

	tokenStream.removeValidSemicolon();

	const std::size_t argumentCount = tokenStream.validateMinimumArguments(1);

	if (argumentCount > 3)
		tokenStream.throwError("The amount of arguments exceeds the maximum amount of [ 3 ]");

	for (std::size_t i = 0; i < argumentCount; ++i)
	{
		const std::string method_token = tokenStream.next();

		// Only Caps → GET
		if (isAllCaps(method_token) == false)
			tokenStream.throwError("Method may contain only capital letters [ " + method_token + " ]");

		// Is method valid
		if (method_token != "GET" && method_token != "POST" && method_token != "DELETE")
			tokenStream.throwError("Found an unsupported Method [ " + method_token + " ]");

		location.addAllowedMethod(toMethod(method_token));
	}

	location.markDirective(ALLOWED);
}
