/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   File.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/22 09:48:29 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/14 01:56:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>
#include <string>

class File
{
  private:
	const std::string name;
	const int fd;

  public:
	File(const std::string name, const int fd);

	const std::string getName() const;
	int getFd() const;
};

std::ostream &operator<<(std::ostream &out, const File &file);

#endif
