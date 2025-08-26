/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RequestParser.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 09:33:08 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/26 17:05:13 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include "http/HttpRequest.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

	// Only consturct with Limits
	RequestParser(const RequestParser &) = delete;
	RequestParser &operator=(const RequestParser &) = delete;
	RequestParser(RequestParser &&) = delete;
	RequestParser &operator=(RequestParser &&) = delete;

#ifdef UNIT_TEST
  public:
#else
  private:
#endif
	bool splitSections(const std::string &raw, std::string &startLine, std::string &headerBlock, std::string &body,
					   RequestParseStatus &status) const;

	bool parseStartLine(const std::string &startLine, HttpRequest &out, RequestParseStatus &status) const;

  public:
	explicit RequestParser(RequestLimits limits);

	RequestLimits getLimits(void);
	HttpRequest parse(std::string);
};

#endif
