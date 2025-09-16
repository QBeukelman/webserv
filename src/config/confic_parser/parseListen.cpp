/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseListen.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:06:36 by hein          #+#    #+#                 */
/*   Updated: 2025/09/16 13:52:11 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"
#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

static void validateColon(std::string &token, TokenStream &tokenStream)
{
	std::size_t first = token.find(':');

	if (first != token.rfind(':'))
	{
		tokenStream.throwError("Found multiple colon [ : ] in a single argument");
	}
	else if (first == std::string::npos)
	{
		tokenStream.throwError("No colon [ : ] found in argument");
	}
}

static void validateHost(std::string &token, TokenStream &tokenStream)
{
	toLower(token);
	if (token == "localhost")
	{
		token = "127.0.0.1";
		return;
	}

	std::stringstream ss(token);
	std::string octet;
	int parts = 0;
	int value;

	while (std::getline(ss, octet, '.'))
	{
		if (octet.empty())
			tokenStream.throwError("Found an empty octet in the IP address");
		if (!isDigitString(octet))
			tokenStream.throwError("Found non-numeric value in an octet of the IP address");
		value = std::stoi(octet);
		if (value > 255)
			tokenStream.throwError("IP octet out of range. Expected [ 0 - 255 ]");
		++parts;
	}
	if (parts != 4)
	{
		tokenStream.throwError("Unexpected amount of IP parts. Expected [ 4 ] parts");
	}
}

static unsigned short validatePort(const std::string &token, TokenStream &tokenStream)
{
	if (!isDigitString(token))
	{
		tokenStream.throwError("Found non-numeric value in Port part of the argument");
	}
	int port = std::stoi(token);
	if (port > 65535)
	{
		tokenStream.throwError("Port value is out of range. Expected [ 0 - 65535 ]");
	}
	return (static_cast<unsigned short>(port));
}

ListenEndpoint createListen(const std::string &token, TokenStream &tokenStream)
{
	ListenEndpoint listen;
	std::stringstream ss(token);
	std::string label;

	std::getline(ss, label, ':');

	validateHost(label, tokenStream);

	listen.host = label;

	std::getline(ss, label, ':');

	listen.port = validatePort(label, tokenStream);

	return (listen);
}

void ConfigParser::parseListen(Server &server, TokenStream &tokenStream)
{
	tokenStream.removeValidSemicolon();

	std::size_t argumentCount = tokenStream.validateMinimumArguments(1);

	for (std::size_t i = 0; i < argumentCount; ++i)
	{
		std::string token = tokenStream.next();

		validateColon(token, tokenStream);

		if (!server.setListen(createListen(token, tokenStream)))
		{
			tokenStream.throwError("Duplicate Listens are not allowed");
		}
	}
	server.markDirective(LISTEN);
}