/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/19 12:05:14 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigParser.hpp"
#include "log/Logger.hpp"

#include <iostream>

ConfigParser::ConfigParser()
{
	Logger::debug("Created Parser");
}

Config ConfigParser::parse(const std::string &path)
{
	(void)(path);
	Config config;
	config.port = 4242;
	return config;
}
