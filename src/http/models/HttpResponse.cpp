/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 09:54:50 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/08 10:37:38 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/HttpResponse.hpp"
#include "log/printUtils.hpp"

std::string serializeResponse(const HttpResponse &response)
{
	std::ostringstream out;

	// Status line
	out << toStatusLine(response.httpStatus);

	// Headers
	for (std::map<std::string, std::string>::const_iterator it = response.headers.begin(); it != response.headers.end();
		 ++it)
	{
		out << it->first << ": " << it->second << "\r\n";
	}

	// End of headers
	out << "\r\n";

	// Body
	out << response.body;

	return (out.str());
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
