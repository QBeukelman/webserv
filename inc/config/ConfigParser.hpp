/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:27 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/02 13:46:53 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>

#include "ServerConfig.hpp"

class ConfigParser
{
  public:
	ConfigParser();
	ServerConfig parse(const std::string &path);
};

#endif // CONFIGPARSER_HPP
