/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 14:49:13 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/02 15:52:40 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "http/models/HttpMethod.hpp"

#include <iostream>
#include <ostream>
#include <set>
#include <string>

class Location
{
  public:
	Location();

	std::string path;
	std::string root;
	bool has_redirects;
	std::set<HttpMethod> allowed_methods;

	bool hasMethod(const HttpMethod) const;
};

std::ostream &operator<<(std::ostream &out, const Location &location);

#endif
