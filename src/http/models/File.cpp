/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   File.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/22 09:48:17 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/22 09:54:03 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/File.hpp"

File::File(const std::string name, const int fd) : name(name), fd(fd) {};

const std::string File::getName() const
{
	return (this->name);
}

const int File::getFd() const
{
	return (this->fd);
}
