/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handleGet.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/23 08:26:52 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/12 19:39:48 by quentinbeuk   ########   odam.nl         */
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

// RESOLVE FILE PATH
// ____________________________________________________________________________
static std::string buildFilePath(const Location &location, const std::string &request_path)
{
	std::string path_prefix = location.path_prefix;
	std::string relative =
		request_path.size() >= path_prefix.size() ? request_path.substr(path_prefix.size()) : request_path;

	if (!relative.empty() && relative[0] == '/')
		relative.erase(0, 1);

	std::string root = location.getRoot();
	if (!root.empty() && root[root.size() - 1] != '/')
		root.push_back('/');

	return (root + relative);
}

static bool isDirectory(const std::string &path)
{
	struct stat st{};
	if (::stat(path.c_str(), &st) != 0)
		return (false);
	return (S_ISDIR(st.st_mode));
}

static std::string joinPath(std::string base, std::string name)
{
	if (!base.empty() && base.back() != '/')
		base.push_back('/');
	if (!name.empty() && name.front() == '/')
		name.erase(name.begin());
	return (base + name);
}

static std::string selectIndexFile(const std::string &directory, const std::vector<std::string> &index_files)
{
	for (const std::string &file : index_files)
	{
		std::string joined_path = joinPath(directory, file);
		if (::access(joined_path.c_str(), R_OK) == 0)
			return (joined_path);
	}
	return ("");
}

// GET
// ____________________________________________________________________________
HttpResponse RequestHandler::handleGet(const HttpRequest &request, const Location &location) const
{
	// 1) Resolve filepath
	std::string file_path = buildFilePath(location, request.path);
	Logger::debug("RequestHandler::handleGet() → Will open file with path: " + file_path);

	// 2) If directory -> try indexes
	if (isDirectory(file_path))
	{
		Logger::info("RequestHandler::handleGet → File path is Directory: " + file_path);

		// 2.1) Search for indexes
		std::string index_path = selectIndexFile(file_path, location.getIndexFiles());
		if (index_path.empty())
		{
			// 2.2) Try Auto-Index
			if (location.getAutoindex() == true)
				return (generateAutoIndexResponse(file_path));
		}

		Logger::info("RequestHandler::handleGet() → Using index file, found directory as path");
		file_path = index_path; // Serve an index file
	}

	// 3) Open file
	const int fd = ::open(file_path.c_str(), O_RDONLY);
	if (fd < 0)
	{
		int e = errno;
		Logger::error("RequestHandler::handleGet() → open failed: " + file_path + " (" + std::strerror(e) + ")");

		switch (e)
		{
		case EACCES: // permission denied
		case EPERM:
			return makeError(STATUS_FORBIDDEN, "Permission denied");

		case ENOENT:  // no such file
		case ENOTDIR: // part of path is not dir
			return makeError(STATUS_NOT_FOUND, "Not found");

		case EISDIR: // is directory
			return makeError(STATUS_FORBIDDEN, "Is a directory");

		case ENAMETOOLONG:
		case ELOOP:
			return makeError(STATUS_NOT_FOUND, "Not found");

		default:
			break;
		}
	}

	// 4) Read file
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
