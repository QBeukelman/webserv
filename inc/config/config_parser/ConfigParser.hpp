/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:39:27 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/17 09:03:01 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <array>
#include <string>
#include <string_view>

#include "config/Server.hpp"
#include "config/ServerConfig.hpp"
#include "config/config_parser/TokenStream.hpp"

class ConfigParser
{

  private:
	template <std::size_t N>
	int findHandlerIndex(const std::array<std::string_view, N> &allowed, const std::string &currentToken);
	template <std::size_t N>
	int findHandlerIndex(const std::array<std::string_view, N> &allowed, const std::string &currentToken);

	void parseGlobal(ServerConfig &config, TokenStream &tokenStream);
	void parseServer(Server &server, TokenStream &tokenStream);
	void parseLocation(Server &server, TokenStream &tokenStream);
	void throwParsingError(Server &server, TokenStream &tokenStream);

	// Server Directives //

	void parseListen(Server &server, TokenStream &tokenStream);
	void parseName(Server &server, TokenStream &tokenStream);
	void parseIndex(Server &server, TokenStream &tokenStream);

	// Location Directives //

	void parseAllowMethod(Server &server, TokenStream &token);
	void parseAutoIndex(Server &server, TokenStream &token);
	void parseReturn(Server &server, TokenStream &token);
	void parseUpload(Server &server, TokenStream &token);

	// General Directives //

	void parseRoot(Server &server, TokenStream &token);
	void parseErrorPage(Server &server, TokenStream &token);
	void parseMaxBody(Server &server, TokenStream &token);

  public:
	ConfigParser();
	ServerConfig parse(const std::string &path);
};

typedef void (ConfigParser::*Handlers)(Server &, TokenStream &);

template <std::size_t N>
int ConfigParser::findHandlerIndex(const std::array<std::string_view, N> &allowed, const std::string &currentToken)
template <std::size_t N>
int ConfigParser::findHandlerIndex(const std::array<std::string_view, N> &allowed, const std::string &currentToken)
{
	for (std::size_t i = 0; i < N; ++i)
	{
		if (allowed[i] == currentToken)
		{
			return i;
		}
	}
	return (N);
}

// ConfigParserUtils.cpp // Non member free function parsing utilities

bool isalnumString(const std::string &s);
bool isDigitString(const std::string &s);

void toLower(std::string &s);

#endif // CONFIGPARSER_HPP
