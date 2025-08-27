/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigParser.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/11 09:39:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/26 14:26:57 by hein          ########   odam.nl         */
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

ConfigParser::ConfigParser()
{
	Logger::debug("Created Parser");
}

void	trimWhitespace(std::string &line)
{
	size_t	start = line.find_first_not_of(" \t\r\n");
	
	if (start == std::string::npos)
	{
		line.clear();
		return ;
	}
	
	if (start > 0)
	{
		line.erase(0, start);
	}

	size_t	end = line.find_last_not_of(" \t\r\n");

	if (end < (line.length()))
	{
		line.erase(end + 1);
	}
}

bool	nextValidLine(std::ifstream &file, std::string &line)
{
	while (true)
	{
		if (!std::getline(file, line))
		{
			if (file.eof())
				return false;
			else
				throw std::runtime_error("An error occured during reading of the file.");
		}

		trimWhitespace(line);
		
		if (line.empty() || line[0] == '#')
		{
			continue ;
		}
		return true;
	}
}

void	parseGlobal(std::ifstream &file, std::string &line)
{
	static constexpr std::array<const std::string_view, 1> allowed[] = {"server"};
}


Config ConfigParser::parse(const std::string &path)
{
	Config			config;
	std::string		line;

	std::ifstream	file(path.c_str());
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open config file: " + path);
	}
	while (nextValidLine(file, line))
	{
		std::cout << line << std::endl;
	}
	
	config.listen.port = 4242;


	
	return config;
}
