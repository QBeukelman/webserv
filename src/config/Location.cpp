/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 15:42:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/18 15:26:56 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Location.hpp"

// INIT
// ____________________________________________________________________________
Location::Location()
	: path_prefix(""), root(""), has_redirects(false), allowed_methods(std::set<HttpMethod>()), allow_uploads(false)
{
}

Location::Location(std::string path_prefix, std::string root, bool has_redirects, std::set<HttpMethod> allowed_methods)
	: path_prefix(path_prefix), root(root), has_redirects(has_redirects), allowed_methods(allowed_methods),
	  allow_uploads(false)
{
}

// PUBLIC
// ____________________________________________________________________________
bool Location::hasMethod(const HttpMethod method) const
{
	return (allowed_methods.find(method) != allowed_methods.end());
}

/*
 * Add a single CGI to `map<extension, CGI>`
 *
 * Notes:
 * 	- Ignores duplicate keys.
 */
void Location::addCgi(const CGI &cgi)
{
	if (cgis.find(cgi.extension) == cgis.end())
	{
		this->cgis[cgi.extension] = cgi;
	}
}

// STATIC
// ____________________________________________________________________________
static void printAllowedMethods(const std::set<HttpMethod> allowed_methods)
{
	std::set<HttpMethod>::const_iterator it = allowed_methods.begin();

	while (it != allowed_methods.end())
	{
		std::cout << *it;
		++it;
		if (it != allowed_methods.end())
			std::cout << ", ";
	}
}

// CGI
// ____________________________________________________________________________
// TODO: Location getCgiByExtension
std::optional<CGI> Location::getCgiByExtension(std::string requestPath) const
{
	return (std::nullopt);
}

std::ostream &operator<<(std::ostream &out, const Location &location)
{
	out << "\n=== Location ===\n"
		<< "Path: " << location.path_prefix << "\nRoot: " << location.root
		<< "\nHas redirect: " << location.has_redirects << "\nAllowed methods: ";
	printAllowedMethods(location.allowed_methods);
	std::cout << std::endl;
	return (out);
}

// UPLOADS
// ____________________________________________________________________________
void Location::addUploadDirectory(const std::string dir)
{
	if (dir.empty())
		return;

	this->upload_dir = dir;
	this->allow_uploads = true;
}

bool Location::hasUploadsDir(const std::string dir) const
{
	struct stat stat_buffer;

	// Read or write permission about file
	if (stat(dir.c_str(), &stat_buffer) != 0)
	{
		return (false);
	}

	// Try to open file
	if (!S_ISDIR(stat_buffer.st_mode))
	{
		return (false);
	}

	return (true);
}
