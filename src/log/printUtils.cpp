/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 09:58:51 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/08 10:01:33 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log/printUtils.hpp"

void printHeaders(std::ostream &out, std::map<std::string, std::string> headers)
{
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		out << it->first << ": " << it->second << "\n";
	}
}
