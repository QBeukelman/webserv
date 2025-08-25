/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:25:32 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 10:09:04 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "HttpMethod.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "RequestParseStatus.hpp"
#include "RequestParser.hpp"

#include <iostream>
#include <string>

// ? Connection to port -> Does not know where it is?
// TODO: Try to use enum(class) for method

class Request
{
  private:
	HttpRequest request;
	RequestParseStatus parse(const std::string &raw);

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
