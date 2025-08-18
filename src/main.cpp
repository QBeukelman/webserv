/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:26:29 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/18 15:21:35 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigParser.hpp"
#include "log/Logger.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	Logger::setLogLevel(LOG_LEVEL_DEBUG);

	ConfigParser parser;
}
