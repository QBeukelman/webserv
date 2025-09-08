/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpMethod.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 09:13:39 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/03 11:43:02 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMETHOD_HPP
#define HTTPMETHOD_HPP

#include <ostream>
#include <string>

enum class HttpMethod
{
	GET,
	POST,
	DELETE,
	UNKNOWN
};

std::string toStringMethod(HttpMethod);
HttpMethod toMethod(const std::string &);
bool isSupported(const std::string &s);
std::ostream &operator<<(std::ostream &, HttpMethod);

#endif
