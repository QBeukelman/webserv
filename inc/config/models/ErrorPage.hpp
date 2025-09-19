/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ErrorPage.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/17 09:52:01 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/19 08:56:53 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include <string>

struct ErrorPage
{
	unsigned short code;
	std::string path;
};

#endif
