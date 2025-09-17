/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:21:57 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/17 13:23:34 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <string>

/*
 * GCI
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

#endif
