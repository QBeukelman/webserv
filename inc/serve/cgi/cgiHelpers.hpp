/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgiHelpers.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/06 14:25:43 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/09 14:51:00 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHELPERS_HPP
#define CGIHELPERS_HPP

#include "http/RequestHandler.hpp"

enum IO
{
	IO_READ,
	IO_WRITE
};

HttpResponse makeCgiResponse(const std::string &raw);

char **toEnvp(const std::vector<std::string> &envStore, std::vector<char *> &envPtrs);
void pushEnv(std::vector<std::string> &env, const std::string &key, const std::string &value);
std::vector<std::string> buildEnv(const HttpRequest &request, const std::string &script);
std::string joinPaths(std::string root, std::string relative_path);

char **makeArgv(const std::string &exc_path, const std::string &script, std::vector<std::string> &argv_strings,
				std::vector<char *> &pointers);

std::pair<std::map<std::string, std::string>, size_t> parseCgiHeaders(const std::string &data);

std::string joinRootAndScript(const std::string &root, const std::string &script);

#endif
