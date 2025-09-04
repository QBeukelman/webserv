/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequest.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 10:20:05 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/02 13:54:54 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/HttpRequest.hpp"

static void printHeaders(std::ostream &out, const HttpRequest &req)
{
	// Print headers
	for (std::map<std::string, std::string>::const_iterator it = req.headers.begin(); it != req.headers.end(); ++it)
	{
		out << it->first << ": " << it->second << "\n";
	}
}

std::ostream &operator<<(std::ostream &out, const HttpRequest &req)
{
	out << "\n======== Request ========\n"
		<< "Status: " << toStringStatus(req.status) << "\n"
		<< "\n------- START -------\n"
		<< "Method: " << toStringMethod(req.method) << "\n"
		<< "Target: " << req.target << "\n"
		<< "Path: " << req.path << "\n"
		<< "Query: " << req.query << "\n"
		<< "Version: " << req.version << "\n"
		<< "\n------- HEADERS -------\n";

	printHeaders(out, req);

	out << "\n------- BODY -------\n"
		<< "Body: " << req.body << "\n"
		<< std::endl;
	return (out);
}
