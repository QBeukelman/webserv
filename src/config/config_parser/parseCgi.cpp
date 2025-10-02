/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parseCgi.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/01 13:51:39 by hein          #+#    #+#                 */
/*   Updated: 2025/10/02 19:41:44 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"
#include "config/config_parser/ConfigParser.hpp"
#include "config/config_parser/TokenStream.hpp"
#include "log/Logger.hpp"

#include <filesystem>
#include <unistd.h>

static void validateExtension(const std::string &extension, TokenStream &tokenStream, Location &location)
{
	if (extension == ".py")
	{
		if (location.hasDirective(CGI_PY))
		{
			tokenStream.throwError("Duplicate cgi directive with [ .py ] extension is not allowed");
		}
		location.markDirective(CGI_PY);
		return;
	}
	if (extension == ".php")
	{
		if (location.hasDirective(CGI_PHP))
		{
			tokenStream.throwError("Duplicate cgi directive with [ .php ] extension is not allowed");
		}
		location.markDirective(CGI_PHP);
		return;
	}
	tokenStream.throwError("Found invalid cgi extension argument [ " + extension + " ]");
}

static void validateExecutablePath(const std::string &executable, TokenStream &tokenStream)
{
	if (executable.front() != '/' && (executable.compare(0, 2, "./") != 0))
	{
		tokenStream.throwError("Executable path must start with either [ / ] or [ ./ ]");
	}
	if (!std::filesystem::exists(executable))
	{
		tokenStream.throwError("Given Executable does not exist");
	}
	if (!std::filesystem::is_regular_file(executable))
	{
		tokenStream.throwError("Given Executable is not a Regular file");
	}
	if (access(executable.c_str(), R_OK | X_OK) != 0)
	{
		tokenStream.throwError("Given Executable is not executable (requires read and execute permissions)");
	}
}

static void validateWorkingDirectory(const std::string &workingDirectory, TokenStream &tokenStream)
{
	if (workingDirectory.front() != '/' && (workingDirectory.compare(0, 2, "./") != 0))
	{
		tokenStream.throwError("Path to Working Directory must start with either [ / ] or [ ./ ]");
	}
	if (!std::filesystem::exists(workingDirectory))
	{
		tokenStream.throwError("Given Working Directory does not exist");
	}
	if (!std::filesystem::is_directory(workingDirectory))
	{
		tokenStream.throwError("Given Working Directory is not a directory");
	}
	if (access(workingDirectory.c_str(), R_OK | X_OK) != 0)
	{
		tokenStream.throwError("Given Working Directory is not accessible (requires read and execute permissions)");
	}
}

void ConfigParser::parseCgi(Location &location, TokenStream &tokenStream)
{

	CGI cgi;

	tokenStream.removeValidSemicolon();

	tokenStream.validateExpectedArguments(3);

	cgi.extension = tokenStream.next();

	validateExtension(cgi.extension, tokenStream, location);

	cgi.executable_path = tokenStream.next();

	validateExecutablePath(cgi.executable_path, tokenStream);

	cgi.working_directory = tokenStream.next();

	validateWorkingDirectory(cgi.working_directory, tokenStream);

	location.addCgi(cgi);
}