/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:09:45 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/08 10:36:38 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "http/models/HttpStatus.hpp"

#include <map>
#include <ostream>
#include <string>

struct HttpResponse
{
	HttpStatus httpStatus;
	std::map<std::string, std::string> headers;
	std::string body;
};

std::string serializeResponse(const HttpResponse &response);

std::ostream &operator<<(std::ostream &out, const HttpResponse &response);

#endif
