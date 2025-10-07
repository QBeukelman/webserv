/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 14:49:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/07 15:30:56 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "config/config_parser/IConfigBlock.hpp"
#include "config/models/CgiConfig.hpp"
#include "config/models/Redirect.hpp"
#include "http/models/HttpMethod.hpp"
#include "log/Logger.hpp"

#include <algorithm>
#include <dirent.h>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <vector>

/*
 * Configuration block that defines how a certain URL path (or prefix) should be handled.
 * ServerConfig owns vector of Locations.
 * Each Location corresponds to a route (path prefix + allowed methods + maybe CGI config, root, etc.
 *
 * Notes:
 * 	- `allow_uploads` is false untill one is added.
 */
class Location : public IConfigBlock
{
  private:
	unsigned int directiveFlags;
	std::string root;
	std::vector<std::string> index_files;
	Redirect redirect;
	bool autoindex;

	const std::string getFileExtenstion(const std::string &request_path) const;

  public:
	Location();
	Location(std::string path_prefix, std::string root, bool has_redirects, std::set<HttpMethod> allowed_methods);

	std::string path_prefix; // e.g. "/static/" or "/upload"
	bool has_redirects;
	std::set<HttpMethod> allowed_methods;
	std::map<std::string, CgiConfig> cgis; // Key is same as file extension `.py`. Including `.`

	// Allowed Methods
	void addAllowedMethod(const HttpMethod &);

	// Location Prefix
	bool setValidatedPrefix(std::string &prefix);
	const std::string &getPrefix() const;

	// Uploads
	bool allow_uploads;
	std::string upload_dir; // e.g. "/var/www/uploads"

	bool hasMethod(const HttpMethod) const;

	// CGI
	std::optional<CgiConfig> getCgiByExtension(const std::string &requestPath) const;
	void addCgi(const CgiConfig &cgi);

	// Uploads
	void addUploadDirectory(const std::string dir);
	bool hasUploadsDir(std::string dir) const;
	std::string normalizeDirectory(std::string directory) const;

	std::string getPath() const;

	// Redirects
	void setRedirect(const Redirect &redirect);
	Redirect getRedirect(void) const;

	// Autoindex
	void setAutoindex(const bool autoindex);
	bool getAutoindex(void) const;

	// IConfigBlock overrides
	void setRoot(const std::string &root); /* override */
	std::string getRoot(void) const;	   /* override */

	bool addIndexFile(const std::string &index);		/* override */
	std::vector<std::string> getIndexFiles(void) const; /* override */

	void markDirective(unsigned int directive);		  /* override */
	bool hasDirective(unsigned int directive);		  /* override */
	bool requiredDirectives(unsigned int directives); /* override */
};

std::ostream &operator<<(std::ostream &out, const Location &location);

#endif
