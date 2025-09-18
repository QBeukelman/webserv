/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/18 11:13:05 by hein          ########   odam.nl         */
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
	serverHandlers = {{"listen", &ConfigParser::parseListen},
					  {"server_name", &ConfigParser::parseName},
					  {"root", &ConfigParser::parseRoot<Server>},
					  {"index", &ConfigParser::parseIndex<Server>},
					  {"client_max_body_size", &ConfigParser::parseMaxBody},
					  {"error_page", &ConfigParser::parseErrorPage},
					  {"location", &ConfigParser::parseLocation}};

	locationHandlers = {{"allowed_methods", &ConfigParser::parseAllowMethod},
						{"root", &ConfigParser::parseRoot<Location>},
						{"index", &ConfigParser::parseIndex<Location>},
						{"autoindex", &ConfigParser::parseAutoIndex},
						{"return", &ConfigParser::parseReturn},
						{"upload_store", &ConfigParser::parseUpload}};
}

void ConfigParser::parseGlobal(ServerConfig &config, TokenStream &tokenStream)
{
	while (true)
	{
		Server server;

		std::string token = tokenStream.next();

		if (token == "server")
		{
			parseServer(server, tokenStream);
		}
		else
		{
			tokenStream.throwError("Found unknown Directive [ " + token + " ]");
		}

		config.addServer(server);
	}
}

void ConfigParser::parseServer(Server &server, TokenStream &tokenStream)
{

	tokenStream.expectOpenBracket(tokenStream.next());

	std::string token = tokenStream.next();

	while (!tokenStream.awaitClosingBracket(token))
	{
		auto index = serverHandlers.find(token);

		if (index != serverHandlers.end())
		{
			(this->*(index->second))(server, tokenStream);
		}
		else
		{
			tokenStream.throwError("Found unknown Directive [ " + token + " ]");
		}

		token = tokenStream.next();
	}

	if (server.requiredDirectives(NAME | LISTEN | ROOT))
	{
		std::cout << server << std::endl;
	}
	exit(1);

	// validate server block //
}

void ConfigParser::parseLocation(Server &server, TokenStream &tokenStream)
{
	Location newLocation;

	tokenStream.expectOpenBracket(tokenStream.next());

	std::string token = tokenStream.next();

	while (!tokenStream.awaitClosingBracket(token))
	{
		auto index = locationHandlers.find(token);

		if (index != locationHandlers.end())
		{
			(this->*(index->second))(newLocation, tokenStream);
		}
		else
		{
			tokenStream.throwError("Found unknown Directive [ " + token + " ]");
		}

		token = tokenStream.next();
	}
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
