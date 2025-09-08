/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseListen.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:06:36 by hein          #+#    #+#                 */
/*   Updated: 2025/09/08 14:29:02 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/configparser/ConfigParser.hpp"
#include "config/configparser/TokenStream.hpp"
#include "log/Logger.hpp"

void	ConfigParser::parseListen(ServerConfig &config, TokenStream &token)
{
	Logger::debug("Parsing Listen");
}