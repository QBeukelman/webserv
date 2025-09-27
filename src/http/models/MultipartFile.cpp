/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   MultipartFile.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/26 22:07:23 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/26 22:11:41 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/MultipartFile.hpp"

std::ostream &operator<<(std::ostream &out, MultipartFile multipartFile)
{
	out << "===MultipartFile===\n"
		<< "Title: " << multipartFile.name << "\nExtension: " << multipartFile.mime.getExtension();
	return (out);
}
