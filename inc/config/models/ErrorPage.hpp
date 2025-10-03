/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ErrorPage.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/17 09:52:01 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/02 20:43:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include "http/models/HttpStatus.hpp"
#include <string>

struct ErrorPage
{
	HttpStatus httpStatus;
	std::string path;

	ErrorPage() : httpStatus(toHttpStatus(0)), path("") {};
};

#endif
