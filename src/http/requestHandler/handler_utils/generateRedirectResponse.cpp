/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   generateRedirectResponse.cpp                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/02 09:22:20 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/10 10:53:23 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

static std::string makeHtmlBody(const std::string &htmlHeader, HttpStatus status, const std::string &redirect_path)
{
	std::ostringstream oss;

	// === head ===
	oss << htmlHeader;

	// === Body ===
	oss << "<body>";

	oss << "<div class=\"spaced-section\">";

	oss << "<h1>" << std::to_string(status) << "</h1>";

	oss << "<p>Redirecting to <a href=\"" << redirect_path << "\">" << redirect_path << "</a></p>\n";

	oss << "<div class=\"center-btn\">";
	oss << "<a href=\"../../index.html\" class=\"back-home-button\">← Back to Home</a>";
	oss << "</div>";

	oss << "</div>";

	oss << "</body>";
	oss << "</html>";

	return (oss.str());
}

HttpResponse RequestHandler::generateRedirectResponse(const Location &location) const
{
	HttpResponse response;

	response.httpStatus = location.getRedirect().httpStatus;

	response.headers["Location"] = location.getRedirect().path;
	response.headers["Content-Type"] = "text/html; charset=UTF-8";

	response.body = makeHtmlBody(makeHtmlPageHeader(), location.getRedirect().httpStatus, location.getRedirect().path);
	response.headers[KEY_CONTENT_LENGTH] = std::to_string(response.body.size());

	return (response);
}
