/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:27 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/29 12:18:24 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>

#include "config/Config.hpp"

typedef struct	tokenCursor
{
	std::string line;
}				tokenCursor;

class ConfigParser
{

  private:

	void	parseGlobal(Config &config, tokenCursor &cursor);
	void	parseServer(Config &config, tokenCursor &cursor);
	void	parseLocation(Config &config, tokenCursor &cursor);
	void	throwParsingError(Config &config, tokenCursor &cursor);

	// Server Directives //

	void	parseListen(Config &config, tokenCursor &cursor);
	void	parseName(Config &config, tokenCursor &cursor);
	void	parseIndex(Config &config, tokenCursor &cursor);
	
	// Location Directives //

	void	parseAllowMethod(Config &config, tokenCursor &cursor);
	void	parseAutoIndex(Config &config, tokenCursor &cursor);
	void	parseReturn(Config &config, tokenCursor &cursor);
	void	parseUpload(Config &config, tokenCursor &cursor);
	
	// General Directives //
	
	void	parseRoot(Config &config, tokenCursor &cursor);
	void	parseErrorPage(Config &config, tokenCursor &cursor);
	void	parseMaxBody(Config &config, tokenCursor &cursor);

  public:

	ConfigParser();
	Config parse(const std::string &path);
};

typedef void (ConfigParser::*Handlers)(Config&, tokenCursor&);

#endif // CONFIGPARSER_HPP
