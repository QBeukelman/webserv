/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/16 13:43:30 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/config_parser/ConfigParser.hpp"
#include "config/Server.hpp"
#include "config/ServerConfig.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string_view>

ConfigParser::ConfigParser()
{
	Logger::debug("Created Parser");
}

void ConfigParser::throwParsingError(Server &server, TokenStream &tokenStream)
{
	tokenStream.throwError("unknown Directive");
}

void ConfigParser::parseGlobal(ServerConfig &config, TokenStream &tokenStream)
{
	static const std::array<std::string_view, 1> allowed = {"server"};
	static const std::array<Handlers, 2> handler = {&ConfigParser::parseServer, &ConfigParser::throwParsingError};

	while (true)
	{
		Server server;

		std::string currentToken = tokenStream.next();

		int index = findHandlerIndex(allowed, currentToken);

		(this->*handler[index])(server, tokenStream);

		config.addServer(server);
	}
}

void ConfigParser::parseServer(Server &server, TokenStream &tokenStream)
{
	static const std::array<std::string_view, 7> allowed = {"listen", "server_name", "root",	"client_max_body_size",
															"index",  "error_page",	 "location"};
	static const std::array<Handlers, 8> handler = {&ConfigParser::parseListen,	  &ConfigParser::parseName,
													&ConfigParser::parseRoot,	  &ConfigParser::parseMaxBody,
													&ConfigParser::parseIndex,	  &ConfigParser::parseErrorPage,
													&ConfigParser::parseLocation, &ConfigParser::throwParsingError};

	tokenStream.expectOpenBracket(tokenStream.next());

	std::string currentToken = tokenStream.next();

	while (!tokenStream.awaitClosingBracket(currentToken))
	{
		int index = findHandlerIndex(allowed, currentToken);

		(this->*handler[index])(server, tokenStream);

		currentToken = tokenStream.next();
	}

	if (server.requiredDirectives(NAME | LISTEN | ROOT))
	{
		server.printListens();
		server.printNames();
		server.printRoot();
	}
	exit(1);

	// validate server block //
}

void ConfigParser::parseLocation(Server &server, TokenStream &tokenStream)
{
	static const std::array<std::string_view, 7> allowed = {
		"allowed_methods", "root", "client_max_body_size", "autoindex", "error_page", "return", "upload_store"};
	static const std::array<Handlers, 8> handler = {&ConfigParser::parseAllowMethod, &ConfigParser::parseRoot,
													&ConfigParser::parseMaxBody,	 &ConfigParser::parseAutoIndex,
													&ConfigParser::parseErrorPage,	 &ConfigParser::parseReturn,
													&ConfigParser::parseUpload,		 &ConfigParser::throwParsingError};

	tokenStream.expectOpenBracket(tokenStream.next());
}

ServerConfig ConfigParser::parse(const std::string &path)
{
	ServerConfig config;
	TokenStream token;

	token.openFile(path);

	try
	{
		parseGlobal(config, token);
	}
	catch (const EndOfFileException &e)
	{
		exit(1);
	}
	catch (const std::exception &e)
	{
		throw;
	}

	// validate config

	return (config);
}
