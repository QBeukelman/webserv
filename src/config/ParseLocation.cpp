/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ParseLocation.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 23:55:27 by hein          #+#    #+#                 */
/*   Updated: 2025/08/26 14:54:45 by hein          ########   odam.nl         */
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

void	parseLocation(std::ifstream &file, std::string &line)
{
	static constexpr std::array<const std::string_view, 6> allowed[] = {"allowed_methods",\
																		"root",\
																		"autoindex",\
																		"error_page",\
																		"return",\
																		"upload_store"};
} 