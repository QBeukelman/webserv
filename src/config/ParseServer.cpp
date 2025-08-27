/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ParseServer.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 23:52:47 by hein          #+#    #+#                 */
/*   Updated: 2025/08/26 14:35:15 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigParser.hpp"
#include "log/Logger.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <span>
#include <array>
#include <string_view>

void	parseServer(std::ifstream &file, std::string &line)
{
	static constexpr std::array<const std::string_view, 7> allowed[] = {"listen",\
																		"server_name",\
																		"root",\
																		"client_max_body_size",\
																		"index",\
																		"error_page",\
																		"location"};
}