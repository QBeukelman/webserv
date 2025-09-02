/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 12:25:32 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/29 10:06:05 by quentinbeuk   ########   odam.nl         */
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

class Request
{
  private:
	ParseStep step;
	ParseContext context;

	RequestParseStatus parse(const char *);

  public:
	// Init
	explicit Request(const char *);

	// Getters & Setters
	ParseStep getParserStep(void) const;
	void setParserStep(ParseStep);
	ParseContext getParseContext(void) const;
	void setParserContext(ParseContext);

	// Use
	HttpResponse execute(/* ServerConfig */) const;
};

#endif
