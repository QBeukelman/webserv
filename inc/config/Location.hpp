/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 14:49:13 by quentinbeuk       #+#    #+#             */
/*   Updated: 2025/09/17 13:51:51 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "config/models/CGI.hpp"
#include "http/models/HttpMethod.hpp"

#include <iostream>
#include <map>
#include <optional>
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
	std::map<std::string, CGI> cgis; // Key is same as file extension `.py`. Including `.`

	bool hasMethod(const HttpMethod) const;
	std::optional<CGI> getCgiByExtension(std::string requestPath) const;

	void addCgi(const CGI &cgi);
};

std::ostream &operator<<(std::ostream &out, const Location &location);

#endif
