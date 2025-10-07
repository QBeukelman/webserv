/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiConfig.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/27 14:37:58 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/27 14:44:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/models/CgiConfig.hpp"

std::ostream &operator<<(std::ostream &out, std::optional<CgiConfig> &cgi)
{
	out << "=== CgiConfig ===\n"
		<< "Extension: " << cgi->extension << "\nExecutable path: " << cgi->executable_path
		<< "\nWorking directory: " << cgi->working_directory;
	return (out);
}
