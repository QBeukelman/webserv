/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Mime.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/24 10:23:58 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/24 11:31:04 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
#define MIME_HPP

#include <map>
#include <string>

/*
 * Multipurpose Internet Mail Extensions
 */
class Mime
{
  public:
	std::string getExtension(const std::string &type_string) const;
	std::string getTypeString(const std::string &extension) const;
};

#endif
