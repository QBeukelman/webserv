/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseRoot.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 12:24:43 by hein          #+#    #+#                 */
/*   Updated: 2025/09/08 14:29:19 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/configparser/ConfigParser.hpp"
#include "config/configparser/TokenStream.hpp"
#include "log/Logger.hpp"

void	ConfigParser::parseRoot(ServerConfig &config, TokenStream &token)
{
	Logger::debug("Parsing Root");
}
