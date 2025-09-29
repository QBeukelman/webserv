/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/29 08:35:18 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/29 08:37:14 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.hpp"

/*
 * `std::string` to lower case.
 *
 * Notes:
 * 	- Only updats `alpha` chars.
 */
std::string toLower(const std::string &s)
{
	std::string out;
	out.reserve(s.size());
	for (size_t i = 0; i < s.size(); ++i)
		out.push_back(std::tolower(static_cast<unsigned char>(s[i])));
	return (out);
}
