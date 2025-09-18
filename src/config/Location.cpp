/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 15:42:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/18 11:42:49 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Location.hpp"

// INIT
// ____________________________________________________________________________
Location::Location() : path_prefix(""), root(""), has_redirects(false), allowed_methods(std::set<HttpMethod>())
{
}

Location::Location(std::string path_prefix, std::string root, bool has_redirects, std::set<HttpMethod> allowed_methods)
	: path_prefix(path_prefix), root(root), has_redirects(has_redirects), allowed_methods(allowed_methods)
{
}

// PUBLIC
// ____________________________________________________________________________
bool Location::hasMethod(const HttpMethod method) const
{
	return (allowed_methods.find(method) != allowed_methods.end());
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

bool Location::setIndex(const std::string &index)
{
	if (std::find(indexFiles.begin(), indexFiles.end(), index) != indexFiles.end())
	{
		return (false);
	}
	indexFiles.push_back(index);
	return (true);
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
