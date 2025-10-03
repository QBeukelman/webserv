#include "log/Logger.hpp"

#include <iostream>

LogLevel Logger::_level = LOG_LEVEL_INFO;
bool Logger::_useColors = true;

void Logger::setLogLevel(LogLevel level)
{
	_level = level;
}

void Logger::setUseColors(bool useColors)
{
	_useColors = useColors;
}

void Logger::error(const std::string &msg)
{
	if (Logger::_level >= LOG_LEVEL_ERROR)
	{
		if (Logger::_useColors)
			std::cerr << RED;
		std::cerr << "ERROR: ";
		if (Logger::_useColors)
			std::cerr << RESET_STYLE;
		std::cerr << msg << std::endl;
	}
}

void Logger::success(const std::string &msg)
{
	if (Logger::_level >= LOG_LEVEL_SUCCESS)
	{
		if (Logger::_useColors)
			std::cout << GREEN;
		std::cout << "SUCCESS: ";
		if (Logger::_useColors)
			std::cout << RESET_STYLE;
		std::cout << msg << std::endl;
	}
}

void Logger::warning(const std::string &msg)
{
	if (Logger::_level >= LOG_LEVEL_WARNING)
	{
		if (Logger::_useColors)
			std::cerr << YELLOW;
		std::cerr << "WARNING: ";
		if (Logger::_useColors)
			std::cerr << RESET_STYLE;
		std::cerr << msg << std::endl;
	}
}

void Logger::info(const std::string &msg)
{
	if (Logger::_level >= LOG_LEVEL_INFO)
	{
		if (Logger::_useColors)
			std::cerr << BLUE;
		std::cerr << "INFO: ";
		if (Logger::_useColors)
			std::cerr << RESET_STYLE;
		std::cerr << msg << std::endl;
	}
}

void Logger::debug(const std::string &msg)
{
	if (Logger::_level >= LOG_LEVEL_DEBUG)
	{
		if (Logger::_useColors)
			std::cerr << MAGENTA;
		std::cerr << "DEBUG: ";
		if (Logger::_useColors)
			std::cerr << RESET_STYLE;
		std::cerr << msg << std::endl;
	}
}

std::string Logger::join(const std::string &s1, const std::string &s2)
{
	std::ostringstream oss;
	oss << s1 << s2;
	return (oss.str());
}
