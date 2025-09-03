/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serve.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 19:43:45 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/09/03 14:37:25 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "log/Logger.hpp"
#include "serve/serve.hpp"

void serve(const ServerConfig &config)
{
	Logger::info(std::string("Starting a server on port ") + std::to_string(config.getPort()) + "...");
	Logger::info("just kiddding, I'm too dummy for now");
}
