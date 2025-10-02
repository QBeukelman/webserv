/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   makeError.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/30 14:52:51 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/30 15:22:46 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

static std::string makeHtmlBody(HttpStatus status, const std::string detail)
{
	std::ostringstream oss;

	// === head ===
	oss << "<!doctype html>\n";
	oss << "<html lang=\"en\">\n";

	oss << "<head>\n";
	oss << "<meta charset=\"utf-8\">\n";
	oss << "<title>Webserv</title>\n";

	oss << "<link href=\"https://fonts.googleapis.com/css2?family=Special+Gothic+Expanded+One&display=swap\" "
		   "rel=\"stylesheet\">";
	oss << "<link href=\"https://fonts.googleapis.com/css2?family=Lora:wght@400;500;700&display=swap\" "
		   "rel=\"stylesheet\">";
	oss << "<link href=\"https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&display=swap\" "
		   "rel=\"stylesheet\">";

	oss << "<link rel=\"stylesheet\" href=\"/website/styles/style.css\">";

	oss << "</head>";

	// === Body ===
	oss << "<body>";

	oss << "<div class=\"spaced-section\">";

	oss << "<h1>" << std::to_string(status) << "</h1>";

	oss << "<p>" << detail << "." << "</p>";

	oss << "<div class=\"center-btn\">";
	oss << "<a href=\"../../index.html\" class=\"back-home-button\">← Back to Home</a>";
	oss << "</div>";

	oss << "</div>";

	oss << "</body>";
	oss << "</html>";

	return (oss.str());
}

HttpResponse RequestHandler::makeError(HttpStatus status, std::string detail) const
{
	// Log error
	Logger::error("RequestHandler::makeError() → " + detail + " → [" + std::to_string(status) + "] " +
				  reasonPhrase(status));

	// Create error response
	HttpResponse response;
	response.httpStatus = status;

	response.headers[KEY_CONTENT_TYPE] = "text/html; charset=UTF-8";
	response.body = makeHtmlBody(status, detail);
	response.headers[KEY_CONTENT_LENGTH] = std::to_string(response.body.size());

	return (response);
}
