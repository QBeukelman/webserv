/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpMethod.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 10:28:28 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/03 11:43:17 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/HttpMethod.hpp"
#include <string>
#include <unordered_map>

namespace
{
const std::unordered_map<HttpMethod, std::string> kMethodToString = {{HttpMethod::GET, "GET"},
																	 {HttpMethod::POST, "POST"},
																	 {HttpMethod::DELETE, "DELETE"},
																	 {HttpMethod::UNKNOWN, "UNKNOWN"}};

const std::unordered_map<std::string, HttpMethod> kStringToMethod = {
	{"GET", HttpMethod::GET}, {"POST", HttpMethod::POST}, {"DELETE", HttpMethod::DELETE}};
} // namespace

std::string toStringMethod(HttpMethod m)
{
	auto it = kMethodToString.find(m);
	if (it != kMethodToString.end())
		return it->second;
	return ("UNKNOWN");
}

HttpMethod toMethod(const std::string &s)
{
	auto it = kStringToMethod.find(s);
	if (it != kStringToMethod.end())
		return it->second;
	return (HttpMethod::UNKNOWN);
}

bool isSupported(const std::string &s)
{
	return (kStringToMethod.find(s) != kStringToMethod.end());
}

std::ostream &operator<<(std::ostream &out, HttpMethod m)
{
	return (out << toStringMethod(m));
}
