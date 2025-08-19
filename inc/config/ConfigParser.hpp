/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:27 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/19 12:04:23 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>

#include "config/Config.hpp"

class ConfigParser
{
  public:
	ConfigParser();
	Config parse(const std::string &path);
};

#endif // CONFIGPARSER_HPP
