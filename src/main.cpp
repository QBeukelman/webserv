/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:26:29 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/02 21:15:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/config_parser/ConfigParser.hpp"
#include "log/Logger.hpp"
#include "serve/WebServer.hpp"

/*
 * Determine full file-path to config file from program args.
 * Otherwise use `config/default.conf`.
 *
 * Returns:
 * 	- Full path to config file, or
 * 	- `""` for too many args.
 */
static std::string getConfigPath(int argc, char **argv)
{
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
		return ("");
	}
	return (pathToConfig);
}

int main(int argc, char **argv)
{
	// Setting logging level
	Logger::setLogLevel(LOG_LEVEL_DEBUG); // Most verbose level
	Logger::setUseColors(true);			  // Turns colors on; Turn them off if you want to redirect the logs to file

	// Getting path to the config from arguments
	const std::string pathToConfig = getConfigPath(argc, argv);
	if (pathToConfig == "")
	{
		Logger::error("Error with path to Configuration file");
		return (1);
	}

	ServerConfig serverConfig;

	// Parse config and run server
	try
	{
		ConfigParser configParser;
		serverConfig = configParser.parse(pathToConfig);
	}
	catch (const std::exception &ex)
	{
		Logger::error(std::string("Failed to parse Configuration file: ") + ex.what());
		return (1);
	}

	// Run server
	WebServer webServer(serverConfig);
	webServer.run();

	return (0);
}
