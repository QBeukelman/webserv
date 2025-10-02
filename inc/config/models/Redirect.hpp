/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redirect.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 08:49:48 by hein              #+#    #+#             */
/*   Updated: 2025/10/02 10:27:51 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_HPP
#define REDIRECT_HPP

#include "http/models/HttpStatus.hpp"

#include <string>

struct Redirect
{
	HttpStatus httpStatus;
	std::string path;

	Redirect() : httpStatus(HttpStatus::STATUS_MOVED_PERMANENTLY), path("") {};
	Redirect(HttpStatus status, std::string path) : httpStatus(status), path(path) {};
};

#endif
