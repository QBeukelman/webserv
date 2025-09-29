/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpRequest.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/29 10:20:05 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/29 08:35:37 by quentinbeuk   ########   odam.nl         */
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

	// out << "\n------- BODY -------\n"
	// 	<< "Body: " << req.body << "\n"
	// 	<< std::endl;
	return (out);
}

/*
 * Case-insensitive header lookup
 *
 * Return:
 * 	- `Value` or "" if not found.
 */
std::string HttpRequest::searchHeader(const std::string &key) const
{
	std::string key_l = toLower(key);

	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		if (toLower(it->first) == key_l)
		{
			return (it->second);
		}
	}
	return ("");
}

std::string HttpRequest::getHostName() const
{
	std::string h = searchHeader("Host");
	if (h.empty())
		return ("");

	size_t c = h.find(':');
	if (c != std::string::npos)
		return (h.substr(0, c));
	return h;
}

unsigned short HttpRequest::getHostPort() const
{
	std::string h = searchHeader("Host");

	size_t c = h.find(':');
	if (c != std::string::npos)
	{
		return (static_cast<unsigned short>(std::atoi(h.substr(c + 1).c_str())));
	}
	return (this->port);
}
