/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HttpResponse.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 09:54:50 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/29 08:32:30 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/HttpResponse.hpp"
#include "log/printUtils.hpp"

std::string HttpResponse::serialize() const
{
	std::ostringstream out;

	// Status line
	out << toStatusLine(this->httpStatus) << "\r\n";

	// Headers
	for (std::map<std::string, std::string>::const_iterator it = this->headers.begin(); it != this->headers.end(); ++it)
	{
		out << it->first << ": " << it->second << "\r\n";
	}

	// End of headers
	out << "\r\n";

	// Body
	out << this->body;

	return (out.str());
}

std::string HttpResponse::searchHeader(const std::string &key) const
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

std::ostream &operator<<(std::ostream &out, const HttpResponse &response)
{
	out << "\n=== Response ===\n"
		<< "Status: " << reasonPhrase(response.httpStatus) << "\n--- HEADERS ---\n";

	printHeaders(out, response.headers);

	out << "\n--- BODY ---\n"
		<< "Body: " << response.body << "\n"
		<< std::endl;

	return (out);
}
