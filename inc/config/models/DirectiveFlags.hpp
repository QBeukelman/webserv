/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirectiveFlags.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/17 09:51:56 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/02 09:23:30 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVEFLAGS_HPP
#define DIRECTIVEFLAGS_HPP

enum DirectiveFlags
{
	LISTEN = 1 << 0,
	NAME = 1 << 1,
	ROOT = 1 << 2,
	INDEX = 1 << 3,
	ERRORPAGE = 1 << 4,
	MAXBODYSIZE = 1 << 5,
	UPLOAD = 1 << 6,
	RETURN = 1 << 7,
	ALLOWED = 1 << 8,
	AUTOINDEX = 1 << 9,
	CGI_PY = 1 << 10,
	CGI_PHP = 1 << 11,
	LOCATION = 1 << 12
};

#endif
