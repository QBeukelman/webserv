/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 15:42:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/01 23:23:23 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Location.hpp"

// INIT
// ____________________________________________________________________________
Location::Location()
	: path_prefix(""), root(""), has_redirects(false), allowed_methods(std::set<HttpMethod>()), allow_uploads(false),
	  autoindex(false)
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

bool Location::setValidatedPrefix(std::string &prefix)
{
	if (prefix.empty() || prefix.front() != '/')
	{
		return (false);
	}
	for (char c : prefix)
	{
		if (!isalnum(static_cast<unsigned char>(c)) && c != '.' && c != '-' && c != '_' && c != '/')
		{
			return (false);
		}
	}
	while (prefix.size() > 1 && prefix.back() == '/')
	{
		prefix.pop_back();
	}
	this->path_prefix = prefix;
	return (true);
}

const std::string &Location::getPrefix() const
{
	return (this->path_prefix);
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

// SET PARSED DATA
// ____________________________________________________________________________

void Location::setRoot(const std::string &root)
{
	this->root = root;
}

bool Location::addIndexFile(const std::string &index)
{
	if (std::find(indexFiles.begin(), indexFiles.end(), index) != indexFiles.end())
	{
		return (false);
	}
	indexFiles.push_back(index);
	return (true);
}

void Location::setReturn(const Redirection &redirect)
{
	this->redirect = redirect;
}

void Location::setAutoindex(const bool autoindex)
{
	this->autoindex = autoindex;
}

// GETTERS
// ____________________________________________________________________________
std::string Location::getRoot(void) const
{
	return (this->root);
}

// BITMASK FLAGG METHODS
// ____________________________________________________________________________
void Location::markDirective(unsigned int directive)
{
	directiveFlags |= directive;
}
bool Location::hasDirective(unsigned int directive)
{
	return ((directiveFlags & directive) != 0);
}
bool Location::requiredDirectives(unsigned int required)
{
	return ((directiveFlags & required) == required);
}

std::ostream &operator<<(std::ostream &out, const Location &location)
{
	out << "\n=== Location ===\n"
		<< "Path: " << location.path_prefix << "\nRoot: " << location.getRoot()
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

bool Location::hasUploadsDir(std::string dir) const
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

/*
 * Returns directory path without leading `/`.
 */
std::string Location::normalizeDirectory(std::string directory) const
{
	if (directory.size() == 1)
		return (directory);

	if (directory[0] == '/' && directory[1] != '/')
	{
		directory = directory.substr(1, directory.size());
	}
	return (directory);
}

// PATH
// ____________________________________________________________________________
std::string Location::getPath() const
{
	return (this->path_prefix);
}
