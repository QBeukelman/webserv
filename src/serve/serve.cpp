/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serve.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 19:43:45 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/08/18 19:47:40 by dkolodze      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "log/Logger.hpp"
#include "serve/serve.hpp"

void serve(const Config &config)
{
	Logger::info(std::string("Starting a server on port ") + std::to_string(config.port) + "...");
	Logger::info("just kiddding, I'm too dummy for now");
}
