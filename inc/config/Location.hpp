/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 14:49:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/18 11:41:37 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "http/models/HttpMethod.hpp"

#include <iostream>
#include <ostream>
#include <set>
#include <string>

/*
 * Configuration block that defines how a certain URL path (or prefix) should be handled.
 * ServerConfig owns vector of Locations.
 * Each Location corresponds to a route (path prefix + allowed methods + maybe CGI config, root, etc.
 */
class Location
{

  private:
	unsigned int directiveFlags;
	std::string root;
	std::vector<std::string> indexFiles;

  public:
	Location();
	Location(std::string path_prefix, std::string root, bool has_redirects, std::set<HttpMethod> allowed_methods);

	std::string path_prefix; // e.g. "/static/" or "/upload"
	bool has_redirects;
	std::set<HttpMethod> allowed_methods;

	bool hasMethod(const HttpMethod) const;

	// Set Parsed Data
	void setRoot(const std::string &root);
	bool setIndex(const std::string &index);

	std::string getRoot(void) const;

	// Bitmask Methods
	void markDirective(unsigned int directive);
	bool hasDirective(unsigned int directive);
	bool requiredDirectives(unsigned int directives);
};

std::ostream &operator<<(std::ostream &out, const Location &location);

#endif
