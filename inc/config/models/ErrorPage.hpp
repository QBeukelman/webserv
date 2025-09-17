/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:52:01 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/17 09:54:47 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include <string>

struct ErrorPage
{
	unsigned short code;
	std::string route;
};

#endif
