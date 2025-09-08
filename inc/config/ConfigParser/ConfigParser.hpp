/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:27 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/04 22:48:07 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>

#include "config/Config.hpp"
#include "config/configparser/TokenStream.hpp"

class ConfigParser
{

  private:

	template<std::size_t N>
	int		findHandlerIndex(const std::array<std::string_view, N>& allowed, const std::string &currentToken);

	void	parseGlobal(Config &config, TokenStream &token);
	void	parseServer(Config &config, TokenStream &token);
	void	parseLocation(Config &config, TokenStream &token);
	void	throwParsingError(Config &config, TokenStream &token);

	// Server Directives //

	void	parseListen(Config &config, TokenStream &token);
	void	parseName(Config &config, TokenStream &token);
	void	parseIndex(Config &config, TokenStream &token);
	
	// Location Directives //

	void	parseAllowMethod(Config &config, TokenStream &token);
	void	parseAutoIndex(Config &config, TokenStream &token);
	void	parseReturn(Config &config, TokenStream &token);
	void	parseUpload(Config &config, TokenStream &token);
	
	// General Directives //
	
	void	parseRoot(Config &config, TokenStream &token);
	void	parseErrorPage(Config &config, TokenStream &token);
	void	parseMaxBody(Config &config, TokenStream &token);

  public:

	ConfigParser();
	Config parse(const std::string &path);
};

typedef void (ConfigParser::*Handlers)(Config &, TokenStream &);

template<std::size_t N>
int	ConfigParser::findHandlerIndex(const std::array<std::string_view, N>& allowed, const std::string &currentToken)
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
