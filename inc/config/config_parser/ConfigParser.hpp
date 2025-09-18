/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:27 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/18 14:33:45 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <map>
#include <string>
#include <string_view>

#include "config/Server.hpp"
#include "config/ServerConfig.hpp"
#include "config/config_parser/TokenStream.hpp"

class ConfigParser
{
  public:
	ConfigParser();
	ServerConfig parse(const std::string &path);

	using ServerHandler = void (ConfigParser::*)(Server &, TokenStream &);
	using LocationHandler = void (ConfigParser::*)(Location &, TokenStream &);

  private:
	void parseGlobal(ServerConfig &config, TokenStream &tokenStream);
	void parseServer(Server &server, TokenStream &tokenStream);
	void parseLocation(Server &server, TokenStream &tokenStream);

	// Server Directives //

	void parseListen(Server &server, TokenStream &tokenStream);
	void parseName(Server &server, TokenStream &tokenStream);
	void parseErrorPage(Server &server, TokenStream &tokenStream);
	void parseMaxBody(Server &server, TokenStream &tokenStream);

	// Location Directives //

	void parseAllowMethod(Location &location, TokenStream &tokenStream);
	void parseAutoIndex(Location &location, TokenStream &tokenStream);
	void parseReturn(Location &location, TokenStream &tokenStream);
	void parseUpload(Location &location, TokenStream &tokenStream);

	// Shared Directives //

	template <typename T> void parseRoot(T &container, TokenStream &token);
	template <typename T> void parseIndex(T &container, TokenStream &tokenStream);

	std::map<std::string_view, ServerHandler> serverHandlers;
	std::map<std::string_view, LocationHandler> locationHandlers;
};

// ConfigParserUtils.cpp // Non member free function parsing utilities

bool isalnumString(const std::string &s);
bool isDigitString(const std::string &s);

void toLower(std::string &s);

#endif // CONFIGPARSER_HPP
