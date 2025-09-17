/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenEndpoint.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 09:51:59 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/17 09:54:30 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENENDPOINT_HPP
#define LISTENENDPOINT_HPP

#include <string>

struct ListenEndpoint
{
	std::string host;	 // "" or "*" → any
	unsigned short port; // e.g. 8080

	ListenEndpoint() : host(""), port(8080)
	{
	}
	ListenEndpoint(const std::string &h, unsigned short p) : host(h), port(p)
	{
	}
};

#endif
