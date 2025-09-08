/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:26:29 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/04 14:00:02 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/configparser/ConfigParser.hpp"
#include "log/Logger.hpp"
#include "serve/serve.hpp"

int main(int argc, char **argv)
{
	// Setting logging level
	Logger::setLogLevel(LOG_LEVEL_DEBUG); // Most verbose level
	Logger::setUseColors(true);			  // Turns colors on; Turn them off if you want to redirect the logs to file

	// Getting path to the config from arguments
	std::string pathToConfig = "";
	if (argc == 1)
	{
		Logger::info("Using default config 'config/default.conf'");
		pathToConfig = "config/default.conf";
	}
	else if (argc == 2)
	{
		Logger::info("Using config '" + std::string(argv[1]) + "'");
		pathToConfig = argv[1];
	}
	else
	{
		Logger::error("Too many arguments; usage: ./webserv [config file]");
		return 1;
	}

	// Parsing config and running the server
	try
	{
		ConfigParser parser;
		Config config = parser.parse(pathToConfig);
		serve(config);
	}
	catch (const std::exception &ex)
	{
		Logger::error(std::string("An exception occured: ") + ex.what());
		return 1;
	}
	return 0;
}
