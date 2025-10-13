/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   makeError.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/30 14:52:51 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 10:35:37 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

static std::string makeHtmlBody(const std::string htmlHeader, HttpStatus status, const std::string detail)
{
	std::ostringstream oss;

	// === head ===
	oss << htmlHeader;

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

static std::string findErrorPage(const std::vector<ErrorPage> &errorPages, const HttpStatus &status)
{
	ErrorPage errorPage;

	for (const auto &e : errorPages)
	{
		// Found error page
		if (e.httpStatus == status)
		{
			errorPage = e;
			break;
		}
		if (&e == &errorPages.back())
		{
			// No matching error page found
			Logger::info("RequestHandler::findErrorPage() → No matching error page in Server");
			return ("");
		}
	}

	// Found error page → try to open it
	const int fd = ::open(errorPage.path.c_str(), O_RDONLY);
	if (fd < 0)
	{
		Logger::info("RequestHandler::makeError() → Could not open error page from Server: " + errorPage.path);
		return ("");
	}

	std::string errorPageBody;
	char buf[4096];
	for (;;)
	{
		size_t n = ::read(fd, buf, sizeof(buf));

		if (n > 0)
			errorPageBody.append(buf, n);
		else if (n == 0)
			break;
		else if (n < 0)
			::close(fd);
	}
	::close(fd);

	return (errorPageBody);
}

HttpResponse RequestHandler::makeError(const HttpStatus status, std::string detail) const
{
	// Log error
	Logger::error("RequestHandler::makeError() → " + detail + " → [" + std::to_string(status) + "] " +
				  reasonPhrase(status));

	// Create error response
	HttpResponse response;
	response.httpStatus = status;
	response.headers[KEY_CONTENT_TYPE] = "text/html; charset=UTF-8";

	// Use error page from config
	const std::string serverErrorPage = findErrorPage(server.getErrorPages(), status);
	if (serverErrorPage.empty() == false)
	{
		Logger::info("RequestHandler::makeError() → Returning error page from Server");

		response.body = serverErrorPage;
		response.headers[KEY_CONTENT_LENGTH] = std::to_string(response.body.size());
		return (response);
	}

	// Generate default error page
	response.body = makeHtmlBody(makeHtmlPageHeader(), status, detail);
	response.headers[KEY_CONTENT_LENGTH] = std::to_string(response.body.size());

	return (response);
}
