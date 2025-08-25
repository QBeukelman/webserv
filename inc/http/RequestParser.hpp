/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 09:33:08 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 10:09:28 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "http/HttpRequest.hpp"

#include <cstddef>
#include <iostream>
#include <string>

struct RequestLimits
{
	size_t max_start_line = 8 * 1024;
	size_t max_header_line = 8 * 1024;
	size_t max_header_size = 64 * 1024; // sum of header lines
	size_t max_body_size = 8 * 1024 * 1024;
};

class RequestParser
{
  private:
	RequestLimits limits;

  public:
	explicit RequestParser(RequestLimits limits);

	// Only consturct with Limits
	RequestParser(const RequestParser &) = delete;
	RequestParser &operator=(const RequestParser &) = delete;
	RequestParser(RequestParser &&) = delete;
	RequestParser &operator=(RequestParser &&) = delete;

	RequestLimits getLimits(void);
	HttpRequest parse(std::string);
};

#endif
