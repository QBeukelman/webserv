/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Logger.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 14:51:32 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/08/26 14:22:00 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

enum LogLevel
{
	LOG_LEVEL_NONE = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARNING = 2,
	LOG_LEVEL_INFO = 3,
	LOG_LEVEL_DEBUG = 4
};

class Logger
{
  public:
	static void setLogLevel(LogLevel level);
	static void setUseColors(bool useColors);

	static void error(const std::string &msg);
	static void warning(const std::string &msg);
	static void info(const std::string &msg);
	static void debug(const std::string &msg);

  private:
	static LogLevel _level;
	static bool _useColors;
};

#endif // LOGGER_HPP
