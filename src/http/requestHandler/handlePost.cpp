/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handlePost.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/23 08:26:56 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/26 23:56:19 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

HttpResponse RequestHandler::handlePost(const HttpRequest &request, const Location &location) const
{
	// Allowed methods
	if (isMethodAllowed(request, location) == false)
		return (makeError(STATUS_METHOD_NOT_ALLOWED, "handlePost()"));

	// Allowed uploads
	if (location.allow_uploads == false || location.upload_dir.empty())
		return (makeError(STATUS_FORBIDDEN, "Uploads not allowed on this Location"));
	if (location.hasUploadsDir(location.normalizeDirectory(location.upload_dir)) == false)
	{
		return (makeError(STATUS_INTERNAL_ERROR, "Upload directory missing, server misconfiguration"));
	}

	// Check body size
	const std::string &body = request.body;
	if (body.empty())
	{
		Logger::error("RequestHandler::handlePost() → Body is empty");
		return (makeError(STATUS_BAD_REQUEST, "Request body is empty"));
	}

	// File Upload
	MultipartFile multipartFile;
	if (request.content_type.getType() == ContentTypeKind::MULTIPART)
	{
		multipartFile = composeMultiPartData(request);
	}

	// Make a file
	const std::string file_name = multipartFile.name + multipartFile.mime.getExtension();
	File file = generateUploadFile(location.normalizeDirectory(location.upload_dir), file_name);

	const std::string raw_data =
		request.content_type.getType() == ContentTypeKind::MULTIPART ? multipartFile.data : body.data();

	if (file.getFd() <= 0)
	{
		Logger::error("RequestHandler::handlePost() → Failed to create/open a new file");
		return (makeError(STATUS_INTERNAL_ERROR, "Failed to create new file"));
	}

	// Write to file
	ssize_t written = ::write(file.getFd(), raw_data.data(), body.size());
	if (written < 0 || static_cast<size_t>(written) != body.size())
	{
		Logger::error("RequestHandler::handlePost() → Writing failed, wrote: " + std::to_string(written) + " bytes");
		return (makeError(STATUS_INTERNAL_ERROR, "Upload write failed"));
	}

	// Generate response
	Logger::info("RequestHandler::handlePost() → Post accepted: wrote " + std::to_string(body.size()) + " bytes");
	HttpResponse response;
	response.httpStatus = STATUS_CREATED;
	response.headers["Content-Length"] = "0";
	response.headers["Location"] = file.getName();
	return (response);
}
