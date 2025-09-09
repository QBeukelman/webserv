/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequestLimits.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/28 17:38:37 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/08 14:00:12 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTLIMITS_HPP
#define HTTPREQUESTLIMITS_HPP

#include <cstddef>

struct HttpRequestLimits
{
	size_t max_start_line = 8 * 1024;
	size_t max_header_line = 8 * 1024;
	size_t max_header_size = 64 * 1024; // sum of header lines
	size_t max_body_size = 8 * 1024 * 1024;
};

#endif
