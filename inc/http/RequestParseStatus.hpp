/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParseStatus.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 09:19:11 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 09:20:59 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSESTATUS_HPP
#define REQUESTPARSESTATUS_HPP

#include <string>

enum RequestParseStatus
{
	PARSE_OK,
	PARSE_INVALID_METHOD,
	PARSE_INVALID_VERSION,
	PARSE_MALFORMED_REQUEST,
	PARSE_INCOMPLETE
};

std::string to_string(RequestParseStatus status);

#endif
