/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpResponse.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/19 13:09:45 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/29 09:06:08 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "http/models/HttpStatus.hpp"
#include "utils/utils.hpp"

#include <map>
#include <ostream>
#include <string>

class HttpResponse
{
  public:
	HttpStatus httpStatus;
	std::map<std::string, std::string> headers;
	std::string body;

	std::string serialize(/* this */) const;
	std::string searchHeader(const std::string &key) const;
};

std::ostream &operator<<(std::ostream &out, const HttpResponse &response);

#endif
