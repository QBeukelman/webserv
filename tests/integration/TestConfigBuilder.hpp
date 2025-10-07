/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TestConfigBuilder.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/15 08:59:53 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/07 15:27:36 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"
#include "config/ServerConfig.hpp"

struct TestConfigBuilder
{
	Server server;
	bool rootAdded;
	std::vector<ListenEndpoint> listenEndpoints;

	// Defaults for Location
	std::string path_prefix;
	std::string root_dir;
	bool has_redirects;
	Redirect redirect;
	std::set<HttpMethod> allowed;
	std::string upload_loc;
	CgiConfig cgi_local;

	TestConfigBuilder()
		: server("Test Server"), rootAdded(false), path_prefix("/"), root_dir("/tmp"), has_redirects(false),
		  redirect(Redirect(HttpStatus::STATUS_MOVED_PERMANENTLY, "/"))
	{
	}

	// ListenEndpoint
	TestConfigBuilder &listen(const std::string &ip, unsigned short port)
	{
		if (port != 0 && port < 1024)
		{
			throw std::runtime_error("Refusing to bind privileged port (<1024) in tests");
		}
		listenEndpoints.push_back(ListenEndpoint(ip, port));
		return (*this);
	}

	// Location
	TestConfigBuilder &new_root(const std::string &root)
	{
		root_dir = root;
		return (*this);
	}
	TestConfigBuilder &new_prefix(const std::string &prefix)
	{
		path_prefix = prefix;
		return (*this);
	}
	TestConfigBuilder &new_has_redirects(bool on)
	{
		has_redirects = on;
		return (*this);
	}
	TestConfigBuilder &new_redirect(Redirect redirect_param)
	{
		redirect = redirect_param;
		return (*this);
	}
	TestConfigBuilder &allow(HttpMethod m)
	{
		allowed.insert(m);
		return (*this);
	}
	TestConfigBuilder &disallow(HttpMethod m)
	{
		allowed.erase(m);
		return (*this);
	}
	TestConfigBuilder &upload_location(std::string upload)
	{
		upload_loc = upload;
		return (*this);
	}
	TestConfigBuilder &cgi(CgiConfig new_cgi)
	{
		cgi_local = new_cgi;
		return (*this);
	}

	// Add Location
	TestConfigBuilder &addLocation(const Location &loc)
	{
		server.addLocation(loc);
		rootAdded = true;
		return (*this);
	}

	// Build ServerConfig
	ServerConfig build()
	{
		// 1) listeners
		server.setListens(listenEndpoints);

		// 2) At least one Location
		if (!rootAdded)
		{
			Location rootLoc(path_prefix, root_dir, has_redirects, allowed);
			rootLoc.addUploadDirectory(upload_loc);
			rootLoc.addCgi(cgi_local);
			server.addLocation(rootLoc);
			rootAdded = true;
		}

		// 3) Assemble ServerConfig
		ServerConfig config;
		config.addServer(server);
		return (config);
	}
};
