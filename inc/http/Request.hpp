/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 12:25:32 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/24 11:04:57 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

#include "HttpMethod.hpp"
#include "HttpResponse.hpp"

#include <iostream>

// ? Connection to port -> Does not know where it is?
// TODO: Try to use enum(class) for method

/*
	HttpResponse: Immutable once parced
*/
struct HttpRequest
{
	HttpMethod method;	 // "GET", "POST", "DELETE"
	std::string target;	 // "/path?query=..."
	std::string path;	 // "/path"
	std::string query;	 // "a=1&b=2"
	std::string version; // "HTTP/1.1"
	std::string body;
	/*Parse status*/
};

class Request
{
  private:
	HttpRequest request;
	bool parse(const std::string &raw);

  public:
	// No copying needed
	Request() = delete;
	Request(const Request &other) = delete;
	Request &operator=(const Request &other) = delete;

	// Init
	explicit Request(std::string); // -> Better not to throw in `hot path`

	// Read & Update
	HttpRequest getRequest(void) const;
	void setRequest(const HttpRequest newRequest);

	// Use
	HttpResponse execute(/* ServerConfig */) const;
};

std::ostream &operator<<(std::ostream &out, const Request &request);

#endif
