/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 09:39:27 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/15 09:21:13 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <array>
#include <string>
#include <string_view>

#include "config/ServerConfig.hpp"
#include "config/config_parser/TokenStream.hpp"

class ConfigParser
{

  private:
	template <std::size_t N>
	int findHandlerIndex(const std::array<std::string_view, N> &allowed, const std::string &currentToken);

	void parseGlobal(ServerConfig &config, TokenStream &token);
	void parseServer(ServerConfig &config, TokenStream &token);
	void parseLocation(ServerConfig &config, TokenStream &token);
	void throwParsingError(ServerConfig &config, TokenStream &token);

	// Server Directives //

	void parseListen(ServerConfig &config, TokenStream &token);
	void parseName(ServerConfig &config, TokenStream &token);
	void parseIndex(ServerConfig &config, TokenStream &token);

	// Location Directives //

	void parseAllowMethod(ServerConfig &config, TokenStream &token);
	void parseAutoIndex(ServerConfig &config, TokenStream &token);
	void parseReturn(ServerConfig &config, TokenStream &token);
	void parseUpload(ServerConfig &config, TokenStream &token);

	// General Directives //

	void parseRoot(ServerConfig &config, TokenStream &token);
	void parseErrorPage(ServerConfig &config, TokenStream &token);
	void parseMaxBody(ServerConfig &config, TokenStream &token);

  public:
	ConfigParser();
	ServerConfig parse(const std::string &path);
};

typedef void (ConfigParser::*Handlers)(ServerConfig &, TokenStream &);

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
	return N;
}

#endif // CONFIGPARSER_HPP
