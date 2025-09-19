/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 14:49:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/18 15:02:39 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "config/models/CGI.hpp"
#include "http/models/HttpMethod.hpp"

#include <dirent.h>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <set>
#include <string>
#include <sys/stat.h>

/*
 * Configuration block that defines how a certain URL path (or prefix) should be handled.
 * ServerConfig owns vector of Locations.
 * Each Location corresponds to a route (path prefix + allowed methods + maybe CGI config, root, etc.
 *
 * Notes:
 * 	- `allow_uploads` is false untill one is added.
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

	// Uploads
	bool allow_uploads;
	std::string upload_dir; // e.g. "/var/www/uploads"

	bool hasMethod(const HttpMethod) const;

	// CGI
	std::optional<CGI> getCgiByExtension(std::string requestPath) const;
	void addCgi(const CGI &cgi);

	// Uploads
	void addUploadDirectory(const std::string dir);
	bool hasUploadsDir(const std::string dir) const;
};

std::ostream &operator<<(std::ostream &out, const Location &location);

#endif
