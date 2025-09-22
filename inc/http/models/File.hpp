/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   File.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/22 09:48:29 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/22 09:50:42 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
#define FILE_HPP

#include <string>

class File
{
  private:
	const std::string name;
	const int fd;

  public:
	File(const std::string name, const int fd);

	const std::string getName() const;
	const int getFd() const;
};

#endif
