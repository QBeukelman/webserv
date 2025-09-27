/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequest.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 10:20:05 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/24 15:06:42 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/HttpRequest.hpp"
#include "log/printUtils.hpp"

std::ostream &operator<<(std::ostream &out, const HttpRequest &req)
{
	const bool has_boundary = !req.content_type.getBoundary().empty();

	out << "\n======== Request ========\n"
		<< "Status: " << toStringStatus(req.status) << "\n"
		<< "Method: " << toStringMethod(req.method) << "\n"
		<< "Target: " << req.target << "\n"
		<< "Path: " << req.path << "\n"
		<< "Query: " << req.query << "\n"
		<< "Version: " << req.version << "\n"
		<< "Content-Type: " << toStringContentType(req.content_type.getType());

	if (has_boundary)
	{
		out << "; boundary=" << req.content_type.getBoundary();
	}

	out << "\n\n------- HEADERS -------\n";

	printHeaders(out, req.headers);

	out << "\n------- BODY -------\n"
		<< "Body: " << req.body << "\n"
		<< std::endl;
	return (out);
}
