/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseMaxBody.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:23:45 by hein          #+#    #+#                 */
/*   Updated: 2025/10/07 18:48:36 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

#include <cstddef> // std::size_t
#include <limits>  // std::numeric_limits

#define MIN_BODY_SIZE 1
#define MAX_BODY_SIZE 1ULL * 1024 * 1024 * 1024

static const std::size_t findMultiplier(std::string &token, TokenStream &tokenStream)
{
	std::size_t multiplier = 1;

	char suffix = std::tolower(token.back());

	if (std::isdigit(suffix))
	{
		return (multiplier);
	}
	switch (suffix)
	{
	case 'k':
		multiplier = 1024ULL;
		break;
	case 'm':
		multiplier = 1024ULL * 1024ULL;
		break;
	case 'g':
		multiplier = 1024ULL * 1024ULL * 1024ULL;
		break;
	default:
		tokenStream.throwError("Invalid suffix. Expected [ K - M - G ]");
	}

	token.pop_back();

	return (multiplier);
}

static std::size_t validateMaxBodySize(std::string &token, TokenStream &tokenStream, std::size_t &multiplier)
{
	if (!isDigitString(token))
	{
		tokenStream.throwError("Non numeric characters are not allowed.");
	}

	std::size_t maxBody = std::stoull(token);

	if ((maxBody > std::numeric_limits<size_t>::max() / multiplier) || maxBody * multiplier > MAX_BODY_SIZE)
	{
		tokenStream.throwError("Maximum Body is higher than the limit of [ 2GB ]");
	}
	else if (maxBody * multiplier < MIN_BODY_SIZE)
	{
		tokenStream.throwError("Maximum Body is lower than the limit of [ 1 ]");
	}

	return (maxBody);
}

void ConfigParser::parseMaxBody(Server &server, TokenStream &tokenStream)
{
	if (server.hasDirective(MAXBODYSIZE))
	{
		tokenStream.throwError("Duplicate client_max_body_size is not allowed");
	}

	tokenStream.removeValidSemicolon();

	tokenStream.validateExpectedArguments(1);

	std::string token = tokenStream.next();

	std::size_t multiplier = findMultiplier(token, tokenStream);

	std::size_t maxBody = validateMaxBodySize(token, tokenStream, multiplier);

	server.setMaxBodySize(static_cast<size_t>(maxBody * multiplier));

	server.markDirective(MAXBODYSIZE);
}

void ConfigParser::parseMaxBody(Location &location, TokenStream &tokenStream)
{
	if (location.hasDirective(MAXBODYSIZE))
	{
		tokenStream.throwError("Duplicate client_max_body_size is not allowed");
	}

	tokenStream.removeValidSemicolon();

	tokenStream.validateExpectedArguments(1);

	std::string token = tokenStream.next();

	std::size_t multiplier = findMultiplier(token, tokenStream);

	std::size_t maxBody = validateMaxBodySize(token, tokenStream, multiplier);

	location.setMaxBodySize(static_cast<size_t>(maxBody * multiplier));

	location.markDirective(MAXBODYSIZE);
}