/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgiHelpers.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/06 14:24:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/07 15:39:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/cgi/cgiHelpers.hpp"

// RESPONSES
// ____________________________________________________________________________
/*
 * Build HttpResponse for CGI with Head and Body
 */
HttpResponse makeCgiResponse(const std::string &raw)
{
	// Split Headers / Body
	size_t pos = raw.find("\r\n\r\n");
	std::string head = (pos == std::string::npos) ? raw : raw.substr(0, pos);
	std::string body = (pos == std::string::npos) ? "" : raw.substr(pos + 4);

	HttpResponse response;
	response.httpStatus = HttpStatus::STATUS_OK;

	std::istringstream h(head);
	std::string line;
	while (std::getline(h, line))
	{
		if (line.empty() == false && line.back() == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			break;

		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue;

		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);

		if (value.empty() == false && value[0] == ' ')
			value.erase(0, 1);

		if (key == "Status")
			response.httpStatus = toHttpStatus(std::atoi(value.c_str()));
		else
			response.headers[key] = value;
	}

	response.body = body;
	if (response.headers.find(KEY_CONTENT_LENGTH) == response.headers.end())
		response.headers[KEY_CONTENT_LENGTH] = std::to_string(body.size());
	if (response.headers.find(KEY_CONTENT_TYPE) == response.headers.end())
		response.headers[KEY_CONTENT_TYPE] = "text/html";

	return (response);
}

// ENV
// ____________________________________________________________________________
/*
 * Convert `vector<>` to `**char` for environment variables
 */
char **toEnvp(const std::vector<std::string> &envStore, std::vector<char *> &envPtrs)
{
	envPtrs.clear();

	for (size_t i = 0; i < envStore.size(); ++i)
		envPtrs.push_back(const_cast<char *>(envStore[i].c_str()));
	envPtrs.push_back(NULL);

	return (&envPtrs[0]);
}

/*
 * Add a new `key` & `value` pair to env vector
 */
void pushEnv(std::vector<std::string> &env, const std::string &key, const std::string &value)
{
	std::string new_entry = key + "=" + value;
	env.push_back(new_entry);
}

/*
 * Build list of environment variables needed by `execve()` in cgi.
 *
 * Required:
 * 		GATEWAY_INTERFACE, REQUEST_METHOD, QUERY_STRING,
 *       SERVER_PROTOCOL, SCRIPT_FILENAME, SCRIPT_NAME,
 *       SERVER_NAME, SERVER_PORT
 */
std::vector<std::string> buildEnv(const HttpRequest &request, const std::string &script)
{
	std::vector<std::string> env;
	pushEnv(env, "GATEWAY_INTERFACE", "CGI/1.1");
	pushEnv(env, "REQUEST_METHOD", toStringMethod(request.method));
	pushEnv(env, "QUERY_STRING", request.query);
	pushEnv(env, "SERVER_PROTOCOL", request.version);
	pushEnv(env, "SCRIPT_FILENAME", script);
	pushEnv(env, "SCRIPT_NAME", request.path);

	if (request.searchHeader(KEY_CONTENT_TYPE).empty() == false)
		pushEnv(env, "CONTENT_TYPE", request.searchHeader(KEY_CONTENT_TYPE));
	if (request.body.empty() == false)
		pushEnv(env, "CONTENT_LENGTH", std::to_string(request.body.size()));

	std::string serverName;
	std::string hostName = request.getHostName();

	if (hostName.empty() == false)
		serverName = hostName;
	else
		serverName = "127.0.0.1"; // Fallback

	pushEnv(env, "SERVER_NAME", serverName);
	pushEnv(env, "SERVER_PORT", std::to_string(request.getHostPort()));

	return (env);
}

/*
 * Build args for `execve()`, null turminated.
 *
 * e.g. argv = ["/usr/bin/python3", "/var/www/scripts/form.py", NULL]
 */
char **makeArgv(const std::string &exc_path, const std::string &script, std::vector<std::string> &argv_strings,
				std::vector<char *> &pointers)
{
	argv_strings.clear();
	pointers.clear();

	argv_strings.push_back(exc_path);
	argv_strings.push_back(script);

	for (size_t i = 0; i < argv_strings.size(); ++i)
		pointers.push_back(const_cast<char *>(argv_strings[i].c_str()));
	pointers.push_back(NULL);

	return (&pointers[0]);
}

// PATH
// ____________________________________________________________________________
/*
 * Joins root and path
 *
 * root `/var/www` + path `index.html` = `/var/www/index.html`
 */
std::string joinPaths(std::string root, std::string relative_path)
{
	if (!root.empty() && root[root.size() - 1] != '/')
		root.push_back('/');
	while (!relative_path.empty() && relative_path[0] == '/')
		relative_path.erase(0, 1);
	return (root + relative_path);
}

// PARSE CGI
// ____________________________________________________________________________
/*
 * Trim whitespace LEFT of string
 */
static void trimLeft(std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	str = (first == std::string::npos) ? "" : str.substr(first);
}

/*
 * Trim whitespace RIGHT of string
 */
static void trimRight(std::string &str)
{
	size_t last = str.find_last_not_of(" \t");
	str = (last == std::string::npos) ? "" : str.substr(0, last + 1);
}

/*
 * Extract headers from the raw output of CGI process
 */
std::pair<std::map<std::string, std::string>, size_t> parseCgiHeaders(const std::string &data)
{
	std::map<std::string, std::string> hdrs;
	size_t pos = std::string::npos;

	// Find CRLF
	size_t first_crlf = data.find("\r\n\r\n");
	size_t second_crlf = data.find("\n\n");

	if (first_crlf != std::string::npos)
		pos = first_crlf + 4;
	else if (second_crlf != std::string::npos)
		pos = second_crlf + 2;

	if (pos == std::string::npos)
	{
		// No header
		return {hdrs, 0};
	}

	// Parse headers
	size_t start = 0;
	while (start < pos)
	{
		size_t eol = data.find('\n', start);
		if (eol == std::string::npos || eol >= pos)
			break;
		size_t line_end = (eol > start && data[eol - 1] == '\r') ? eol - 1 : eol;
		std::string line = data.substr(start, line_end - start);
		start = eol + 1;

		if (line.empty())
			break;

		size_t colon = line.find(':');
		if (colon != std::string::npos)
		{
			std::string header_key = line.substr(0, colon);
			std::string header_value = line.substr(colon + 1);

			// trim spaces
			trimLeft(header_key);
			trimRight(header_key);
			trimLeft(header_value);
			trimRight(header_value);

			hdrs[header_key] = header_value;
		}
	}

	return {hdrs, pos};
}
