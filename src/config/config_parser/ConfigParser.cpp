/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/02 22:48:21 by hein          ########   odam.nl         */
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

// CONSTRUCTORS
// ____________________________________________________________________________
ConfigParser::ConfigParser()
{
	serverHandlers = {{"listen", &ConfigParser::parseListen},
					  {"server_name", &ConfigParser::parseName},
					  {"root", &ConfigParser::parseRoot<Server>},
					  {"index", &ConfigParser::parseIndex<Server>},
					  {"client_max_body_size", &ConfigParser::parseMaxBody},
					  {"error_page", &ConfigParser::parseErrorPage},
					  {"location", &ConfigParser::parseLocation}};

	locationHandlers = {{"allow_methods", &ConfigParser::parseAllowMethod},
						{"root", &ConfigParser::parseRoot<Location>},
						{"index", &ConfigParser::parseIndex<Location>},
						{"autoindex", &ConfigParser::parseAutoIndex},
						{"return", &ConfigParser::parseReturn},
						{"upload_store", &ConfigParser::parseUpload},
						{"cgi", &ConfigParser::parseCgi}};
}

// FIND INDEX
// ____________________________________________________________________________
int ConfigParser::findHandlerIndex(const std::vector<std::string> &allowed, const std::string &currentToken)
{
	for (std::size_t i = 0; i < allowed.size(); ++i)
	{
		if (allowed[i] == currentToken)
		{
			return (static_cast<int>(i));
		}
	}
	return (static_cast<int>(allowed.size()));
}

// PARSE HELPERS
// ____________________________________________________________________________
void ConfigParser::parseGlobal(ServerConfig &config, TokenStream &tokenStream)
{
	while (true)
	{
		Server server;

		std::string token = tokenStream.next();
		if (token == "server")
		{
			config.incrementDepth();
			parseServer(server, tokenStream);
		}
		else
		{
			tokenStream.throwError("Found unknown Directive [ " + token + " ]");
		}

		if (server.requiredDirectives(NAME | LISTEN) && server.hasDirective(ROOT | LOCATION))
		{
			config.addServer(server);
			config.decrementDepth();
		}
		else
		{
			tokenStream.throwError("Server Block was closed without the minimum required directives");
		}
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
}

void ConfigParser::parseLocation(Server &server, TokenStream &tokenStream)
{
	Location newLocation;

	tokenStream.validateMinimumArguments(1);

	std::string prefix = tokenStream.next();

	if (!newLocation.setValidatedPrefix(prefix))
	{
		tokenStream.throwError(
			"Location Prefix should be a absolute path and consist of alphanumeric characters or [ . - _ / ]");
	}

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

	if (newLocation.hasDirective(RETURN | ROOT | CGI_PY | CGI_PHP))
	{
		if (!server.addLocation(newLocation))
		{
			tokenStream.throwError("Duplicate Location Prefix Path is not allowed");
		}
		server.markDirective(LOCATION);
	}
	else
	{
		tokenStream.throwError("Locationblock was closed without meaningful Directive present");
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
		if (!config.depthProperlyClosed())
		{
			throw std::runtime_error("EOF reached, but Server Block was not closed properly. Missing [ } ]");
		}
	}
	catch (const std::exception &e)
	{
		throw;
	}

	std::cout << "parsed succesful" << std::endl;

	return (config);
}
