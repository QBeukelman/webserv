/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handlePost.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/23 08:26:56 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/10 11:44:42 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

static bool shouldAppendFileExtension(const std::string &name)
{
	if (name.find('.') == std::string::npos)
		return (true);
	return (false);
}

/*
 * Used for uploads
 */
HttpResponse RequestHandler::handleMultipartPost(const HttpRequest &request, const Location &location) const
{
	MultipartFile multipartFile = composeMultiPartData(request);

	// Make a file
	std::string file_name = multipartFile.name;
	if (shouldAppendFileExtension(multipartFile.name) == true)
		file_name = multipartFile.name + multipartFile.mime.getExtension();

	File file = generateUploadFile(location.normalizeDirectory(location.upload_dir), file_name);

	const std::string raw_data =
		request.content_type.getType() == ContentTypeKind::MULTIPART ? multipartFile.data : request.body.data();

	if (file.getFd() <= 0)
	{
		Logger::error("RequestHandler::handlePost() → Failed to create/open a new file");
		return (makeError(STATUS_INTERNAL_ERROR, "Failed to create new file"));
	}

	// Write to file
	ssize_t written = ::write(file.getFd(), raw_data.data(), raw_data.size());
	if (written < 0 || static_cast<size_t>(written) != raw_data.size())
	{
		Logger::error("RequestHandler::handlePost() → Writing failed, wrote: " + std::to_string(written) + " bytes");
		return (makeError(STATUS_INTERNAL_ERROR, "Upload write failed"));
	}

	// Generate response
	Logger::info("RequestHandler::handlePost() → Post accepted: wrote " + std::to_string(raw_data.size()) + " bytes");
	HttpResponse response;
	response.httpStatus = STATUS_CREATED;
	response.headers[KEY_CONTENT_LENGTH] = "0";
	response.headers["Location"] = file.getName();

	return (response);
}

HttpResponse RequestHandler::handlePost(const HttpRequest &request, const Location &location) const
{
	// Allowed uploads
	if (location.allow_uploads == false || location.upload_dir.empty())
		return (makeError(STATUS_FORBIDDEN, "Uploads not allowed on this Location"));
	if (location.hasUploadsDir(location.normalizeDirectory(location.upload_dir)) == false)
		return (makeError(STATUS_INTERNAL_ERROR, "Upload directory missing, server misconfiguration"));

	// Check body size
	if (request.body.empty())
	{
		Logger::error("RequestHandler::handlePost() → Body is empty");
		return (makeError(STATUS_BAD_REQUEST, "Request body is empty"));
	}

	// File Upload / Multipart
	if (request.content_type.getType() == ContentTypeKind::MULTIPART)
		return (handleMultipartPost(request, location));

	// Do nothing with request
	Logger::info("RequestHandler::handlePost() → Will do nothing with request [200]");

	std::string message = "OK\n";
	message += "len=" + std::to_string(request.body.size()) + "\n";
	message += "body=" + request.body + "\n";

	HttpResponse response;
	response.httpStatus = STATUS_OK; // 200
	response.headers[KEY_CONTENT_TYPE] = "text/plain; charset=UTF-8";
	response.body = message;
	response.headers[KEY_CONTENT_LENGTH] = std::to_string(response.body.size());

	return (response);
}
