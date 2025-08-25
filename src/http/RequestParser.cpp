/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 09:50:01 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 10:10:23 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestParser.hpp"

RequestParser::RequestParser(RequestLimits limits)
{
	this->limits = limits;
}

RequestLimits RequestParser::getLimits(void)
{
	return (this->limits);
}

HttpRequest RequestParser::parse(std::string requestString)
{
	std::cout << requestString;

	return (HttpRequest());
}
