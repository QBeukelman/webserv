/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   File.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/22 09:48:17 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/14 01:56:39 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/File.hpp"

File::File(const std::string name, const int fd) : name(name), fd(fd) {};

const std::string File::getName() const
{
	return (this->name);
}

int File::getFd() const
{
	return (this->fd);
}

std::ostream &operator<<(std::ostream &out, const File &file)
{
	out << "File[" << file.getFd() << "] " << file.getName();
	return (out);
}
