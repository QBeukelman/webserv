/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 15:42:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/02 15:53:42 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Location.hpp"

// INIT
// ____________________________________________________________________________
Location::Location() : path(""), root(""), has_redirects(false), allowed_methods(std::set<HttpMethod>())
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
	for (auto m : allowed_methods)
	{
		std::cout << m << "\n";
	}
}

std::ostream &operator<<(std::ostream &out, const Location &location)
{
	out << "\n==== Location ====\n"
		<< "Path: " << location.path << "Root: " << location.root << "Has redirect: " << location.has_redirects;
	printAllowedMethods(location.allowed_methods);
	return (out);
}
