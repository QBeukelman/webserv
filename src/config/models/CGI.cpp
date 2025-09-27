/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/27 14:37:58 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/27 14:44:16 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/models/CGI.hpp"

std::ostream &operator<<(std::ostream &out, std::optional<CGI> &cgi)
{
	out << "=== CGI ===\n"
		<< "Extension: " << cgi->extension << "\nExecutable path: " << cgi->executable_path
		<< "\nWorking directory: " << cgi->working_directory;
	return (out);
}
