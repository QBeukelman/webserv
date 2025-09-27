/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   MultipartFile.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/26 22:03:18 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/26 22:54:37 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPARTFILE_HPP
#define MULTIPARTFILE_HPP

#include "http/models/Mime.hpp"

#include <iostream>
#include <string>

struct MultipartFile
{
	std::string data;
	std::string name;
	Mime mime;

	MultipartFile() : data(""), name(""), mime() {};
	MultipartFile(std::string data, std::string name, Mime mime) : data(data), name(name), mime(mime) {};
};

std::ostream &operator<<(std::ostream &out, MultipartFile miltipartFile);

#endif
