/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpMethod.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/24 10:28:28 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/08/24 11:04:43 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/HttpMethod.hpp"
#include <string>
#include <unordered_map>

namespace
{
const std::unordered_map<HttpMethod, std::string> kMethodToString = {{HttpMethod::GET, "GET"},
																	 {HttpMethod::POST, "POST"},
																	 {HttpMethod::DELETE_, "DELETE"},
																	 {HttpMethod::UNKNOWN, "UNKNOWN"}};

const std::unordered_map<std::string, HttpMethod> kStringToMethod = {
	{"GET", HttpMethod::GET}, {"POST", HttpMethod::POST}, {"DELETE", HttpMethod::DELETE_}};
} // namespace

std::string to_string(HttpMethod m)
{
	auto it = kMethodToString.find(m);
	if (it != kMethodToString.end())
		return it->second;
	return "UNKNOWN";
}

HttpMethod to_method(const std::string &s)
{
	auto it = kStringToMethod.find(s);
	if (it != kStringToMethod.end())
		return it->second;
	return HttpMethod::UNKNOWN;
}

std::ostream &operator<<(std::ostream &out, HttpMethod m)
{
	return out << to_string(m);
}
