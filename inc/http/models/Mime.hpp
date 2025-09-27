/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Mime.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/24 10:23:58 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/26 15:57:47 by quentinbeuk   ########   odam.nl         */
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
  private:
	std::string extension;
	std::string type_string;

  public:
	Mime();
	Mime(const std::string &type_string);

	std::string getExtension(const std::string &type_string) const;
	std::string getTypeString(const std::string &extension) const;

	std::string getExtension(/* this */) const;
	std::string getTypeString(/* this */) const;
};

#endif
