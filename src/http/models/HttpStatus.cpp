/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpStatus.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/02 15:23:10 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/02 15:41:23 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/HttpStatus.hpp"

std::string toStatusLine(HttpStatus status, const std::string &version)
{
	std::ostringstream oss;
	oss << version << " " << static_cast<int>(status) << " " << reasonPhrase(status) << "\r\n";
	return (oss.str());
}

std::ostream &operator<<(std::ostream &out, HttpStatus status)
{
	out << static_cast<int>(status) << " " << reasonPhrase(status);
	return (out);
}
