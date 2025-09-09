/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseErrorPage.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:22:39 by hein          #+#    #+#                 */
/*   Updated: 2025/09/08 14:48:54 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

void	ConfigParser::parseErrorPage(ServerConfig &config, TokenStream &token)
{
	Logger::debug("Parsing Error Page");
}