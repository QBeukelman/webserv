/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpMethod.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 09:13:39 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/24 11:04:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHOD_HPP
#define HTTPMETHOD_HPP

#include <ostream>
#include <string>
#include <unordered_map>

enum class HttpMethod
{
	GET,
	POST,
	DELETE_,
	UNKNOWN
};

std::string to_string(HttpMethod);
HttpMethod to_method(const std::string &);
std::ostream &operator<<(std::ostream &, HttpMethod);

#endif
