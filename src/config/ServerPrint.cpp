/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerPrint.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/16 13:37:11 by hein          #+#    #+#                 */
/*   Updated: 2025/09/16 13:43:01 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"
#include <iostream>

void Server::printNames()
{
	for (std::string s : names)
	{
		std::cout << s << std::endl;
	}
}

void Server::printListens()
{
	for (ListenEndpoint i : listens)
	{
		std::cout << "Host [ " + i.host + " ] Port [ " + std::to_string(i.port) + " ]" << std::endl;
	}
}

void Server::printRoot()
{
	std::cout << "Root [ " + root + " ]" << std::endl;
}