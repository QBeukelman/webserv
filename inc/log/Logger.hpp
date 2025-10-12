/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Logger.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 14:51:32 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/10/10 10:15:05 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sstream>
#include <string>

namespace
{
const std::string RED = "\033[31m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string GREEN = "\033[32m";
const std::string LIGHT_GRAY = "\033[37m";
const std::string UNDERLINE = "\033[4m";
const std::string RESET_STYLE = "\033[0m";
} // namespace

enum LogLevel
{
	LOG_LEVEL_NONE = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_SUCCESS = 2,
	LOG_LEVEL_WARNING = 3,
	LOG_LEVEL_INFO = 4,
	LOG_LEVEL_DEBUG = 5
};

class Logger
{
  public:
	static void setLogLevel(LogLevel level);
	static void setUseColors(bool useColors);

	static void error(const std::string &msg);
	static void success(const std::string &msg);
	static void warning(const std::string &msg);
	static void info(const std::string &msg);
	static void debug(const std::string &msg);
	static void log(const std::string &msg);

	static std::string join(const std::string &s1, const std::string &s2);

  private:
	static LogLevel _level;
	static bool _useColors;
};

#endif // LOGGER_HPP
