/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:25:32 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/19 13:41:45 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

#include "HttpResponse.hpp"

// ? Connection to port -> Does not know where it is?
// TODO: Try to use enum(class) for method

struct HttpRequest
{
	std::string method;	 // "GET", "POST", "DELETE"
	std::string target;	 // "/path?query=..."
	std::string path;	 // "/path"
	std::string query;	 // "a=1&b=2"
	std::string version; // "HTTP/1.1"
	std::string body;
};

class Request
{
  private:
	HttpRequest req;
	bool parse(const std::string &raw);

  public:
	Request(std::string); // -> throws

	HttpResponse execute(/* ServerConfig */) const;
};

#endif
