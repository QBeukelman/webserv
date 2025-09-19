/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirectiveFlags.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/17 09:51:56 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/19 08:56:28 by quentinbeuk   ########   odam.nl         */
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
	MAXBODYSIZE = 1 << 5
};

#endif
