/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleGet.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/23 08:26:52 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/27 10:24:44 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

static std::string getFileExtension(const std::string &file_path)
{
	size_t dot = file_path.find_last_of('.');
	if (dot == std::string::npos)
		return ("");

	return (file_path.substr(dot));
}

HttpResponse RequestHandler::handleGet(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleGet()"));

	if (std::optional<CGI> cgi = location.getCgiByExtension(request.path))
	{
		// Run CGI(CGI)
		return (HttpResponse());
	}

	// 1) Open file
	const std::string file_path = location.normalizeDirectory(location.getRoot()) + request.path;

	const int fd = open(file_path.c_str(), O_RDONLY);
	if (fd < 0)
	{
		Logger::error("RequestHandler::handleGet() → Could not open file: " + file_path);
		return (makeError(STATUS_NOT_FOUND, "Could not find file"));
	}

	// 2) Read file
	std::string body;
	char buf[4096];

	for (;;)
	{
		ssize_t n = ::read(fd, buf, sizeof(buf));
		if (n > 0)
			body.append(buf, static_cast<size_t>(n));
		else if (n == 0)
			break;
		else if (n < 0)
		{
			// Read failed
			if (errno == EINTR)
				continue;
			Logger::error("RequestHandler::handleGet() → Read failed: " + std::string(std::strerror(errno)));
			::close(fd);
			return (makeError(STATUS_INTERNAL_ERROR, "Read error"));
		}
	}
	::close(fd);

	std::cout << "Body " << body << std::endl;

	// Make response
	// TODO: handleGet() → Stream entire file
	Logger::info("RequestHandler::handleGet() → Get Accepted");
	HttpResponse response;
	Mime mime;

	response.httpStatus = STATUS_OK;
	response.body = body;
	response.headers["Content-Length"] = std::to_string(body.size());
	response.headers["Content-type"] = mime.getTypeString(getFileExtension(file_path));
	return (response);
}
