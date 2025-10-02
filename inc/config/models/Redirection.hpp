/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Redirection.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/01 08:49:48 by hein          #+#    #+#                 */
/*   Updated: 2025/10/01 09:47:57 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_HPP
#define REDIRECTION_HPP

#include <string>

struct Redirection
{
	unsigned short code;
	std::string path;
};

#endif