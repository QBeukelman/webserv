/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleGet.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/23 08:26:52 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/30 15:27:13 by quentinbeuk   ########   odam.nl         */
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

static std::string buildFilePath(const Location &location, const std::string &request_path)
{
	// 1) Substring path after prefix
	std::string relative = request_path.substr(location.path_prefix.size());

	// 2) Join it with root
	std::string file_path = location.getRoot();
	if (file_path.empty() == false && file_path.back() != '/')
		file_path.push_back('/');
	return (file_path + relative);
}

HttpResponse RequestHandler::handleGet(const HttpRequest &request, const Location &location) const
{
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handleGet()"));

	if (std::optional<CGI> cgi = location.getCgiByExtension(request.path))
	{
		Logger::info("RequestHandler::handleGet() → Starting CGI");
		return (handleCgi(request, location, *cgi));
	}

	// 1) Open file
	const std::string file_path = buildFilePath(location, request.path);

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

	// Make response
	// TODO: handleGet() → Stream entire file
	Logger::info("RequestHandler::handleGet() → Get Accepted");
	HttpResponse response;
	Mime mime;

	response.httpStatus = STATUS_OK;
	response.body = body;
	response.headers[KEY_CONTENT_LENGTH] = std::to_string(body.size());
	response.headers[KEY_CONTENT_TYPE] = mime.getTypeString(getFileExtension(file_path));
	return (response);
}
