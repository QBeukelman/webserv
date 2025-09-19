/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TestConfigBuilder.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/14 13:11:08 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/14 18:20:00 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTCONFIGBUILDER_HPP
#define TESTCONFIGBUILDER_HPP

#include "config/Location.hpp"
#include "config/Server.hpp"
#include "config/ServerConfig.hpp"
#include "http/models/HttpMethod.hpp"
#include "serve/Listener.hpp"

struct TestConfigBuilder
{
	Server server = Server("Test Server");
	bool rootAdded;
	std::vector<ListenEndpoint> listenEndpoints;

	std::string path_prefix;
	std::string root_dir;
	bool has_redirects;
	std::set<HttpMethod> allowed;

	// Constructor
	TestConfigBuilder()
		: server("Test Server"), rootAdded(false), path_prefix("/"), root_dir("/temp"), has_redirects(false)
	{
		allowed.insert(HttpMethod::GET);
	}

	// Add a listen endpoint
	TestConfigBuilder &listen(const std::string &ip, unsigned short port)
	{
		listenEndpoints.push_back(ListenEndpoint(ip, port));
		return (*this);
	}

	// Adjust default root location
	TestConfigBuilder &mount(const std::string &p)
	{
		path_prefix = p;
		return (*this);
	}
	TestConfigBuilder &docroot(const std::string &dir)
	{
		root_dir = dir;
		return (*this);
	}
	TestConfigBuilder &redirects(bool on)
	{
		has_redirects = on;
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

	// Add explicid location
	TestConfigBuilder &addLocation(const Location &loc)
	{
		server.addLocation(loc);
		rootAdded = true;
		return (*this);
	}

	// Build a ServerConfig
	ServerConfig build()
	{
		// 1) listeners
		server.setListens(listenEndpoints);

		// 2) ensure at least one Location exists
		if (!rootAdded)
		{
			Location rootLoc(path_prefix, root_dir, has_redirects, allowed);
			server.addLocation(rootLoc);
			rootAdded = true;
		}

		// 3) assemble ServerConfig
		ServerConfig config;
		config.addServer(server);
		return (config);
	}
};

#endif
