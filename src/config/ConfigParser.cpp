/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/03 14:36:53 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigParser.hpp"
#include "log/Logger.hpp"

#include <iostream>

ConfigParser::ConfigParser()
{
	Logger::debug("Created Parser");
}

ServerConfig ConfigParser::parse(const std::string &path)
{
	(void)(path);
	ServerConfig config;
	config.setPort(4242);
	return (config);
}
