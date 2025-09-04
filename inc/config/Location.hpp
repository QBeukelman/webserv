/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 14:49:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/03 14:19:24 by quentinbeuk   ########   odam.nl         */
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
  public:
	Location();
	Location(std::string path_prefix, std::string root, bool has_redirects, std::set<HttpMethod> allowed_methods);

	std::string path_prefix; // e.g. "/static/" or "/upload"
	std::string root;		 // e.g. "/var/www/site"
	bool has_redirects;
	std::set<HttpMethod> allowed_methods;

	bool hasMethod(const HttpMethod) const;
};

std::ostream &operator<<(std::ostream &out, const Location &location);

#endif
