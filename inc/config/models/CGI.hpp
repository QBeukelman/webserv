/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/17 13:21:57 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/27 23:46:03 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <optional>
#include <string>

/*
 * GCI (Common Gateway Interface)
 *
 * Notes:
 * 	- Default working directory is same as location-root
 */
struct CGI
{
	std::string extension;
	std::string executable_path;
	std::string working_directory; // Default is location-root

	CGI() : extension(""), executable_path(""), working_directory("") {};
};

std::ostream &operator<<(std::ostream &out, std::optional<CGI> &cgi);

#endif
