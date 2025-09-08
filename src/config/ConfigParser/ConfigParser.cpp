/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/08 13:30:00 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Config.hpp"
#include "config/configparser/ConfigParser.hpp"
#include "config/configparser/TokenStream.hpp"
#include "log/Logger.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <span>
#include <array>
#include <string_view>

ConfigParser::ConfigParser()
{
	Logger::debug("Created Parser");
}

void	ConfigParser::throwParsingError(Config &config, TokenStream &token)
{
	throw std::runtime_error("Error on line " + token.getLineCount() + ". Unknown directive [ " + token.getCurrentToken() + " ]");
}

void	ConfigParser::parseGlobal(Config &config, TokenStream &token)
{
	static const std::array<std::string_view, 1> allowed = {"server"};

	static const std::array<Handlers, 2> handler = {		&ConfigParser::parseServer,\
															&ConfigParser::throwParsingError};

	while (true)
	{
		std::string currentToken = token.next();
		
		int index = findHandlerIndex(allowed, currentToken);

		(this->*handler[index])(config, token);
	}

}

void	ConfigParser::parseServer(Config &config, TokenStream &token)
{
	static const std::array<std::string_view, 7>allowed = {	"listen",\
															"server_name",\
															"root",\
															"client_max_body_size",\
															"index",\
															"error_page",\
															"location"};
	static const std::array<Handlers, 8> handler = {		&ConfigParser::parseListen,\
															&ConfigParser::parseName,\
															&ConfigParser::parseRoot,\
															&ConfigParser::parseMaxBody,\
															&ConfigParser::parseIndex,\
															&ConfigParser::parseErrorPage,\
															&ConfigParser::parseLocation,\
															&ConfigParser::throwParsingError};

	token.expectOpenBracket(token.next());

	std::string currentToken = token.next();
	
	while (!token.awaitClosingBracket(currentToken))
	{
		int index = findHandlerIndex(allowed, currentToken);

		(this->*handler[index])(config, token);

		currentToken = token.next();
	}

	// validate server block //
}

void	ConfigParser::parseLocation(Config &config, TokenStream &token)
{
	static const std::array<std::string_view, 7> allowed = {"allowed_methods",\
															"root",\
															"client_max_body_size",\
															"autoindex",\
															"error_page",\
															"return",\
															"upload_store"};
	static const std::array<Handlers, 8> handler = {		&ConfigParser::parseAllowMethod,\
															&ConfigParser::parseRoot,\
															&ConfigParser::parseMaxBody,\
															&ConfigParser::parseAutoIndex,\
															&ConfigParser::parseErrorPage,\
															&ConfigParser::parseReturn,\
															&ConfigParser::parseUpload,\
															&ConfigParser::throwParsingError};

	token.expectOpenBracket(token.next());
	
	
}



Config ConfigParser::parse(const std::string &path)
{
	Config			config;
	TokenStream		token;

	token.openFile(path);

	try
	{
		parseGlobal(config, token);
	}
	catch (const EndOfFileException& e)
	{
		exit(1);
	}
	catch (const std::exception& e)
	{
		throw;
	}

	// validate config
		
	return config;
}
