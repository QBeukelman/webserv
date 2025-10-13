/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   composeMultiPartData.cpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/26 14:40:24 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 15:39:25 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

static size_t countBoundaries(const std::string &request_body, const std::string &boundary)
{
	if (boundary.empty())
		return (0);

	size_t count = 0;
	size_t pos = request_body.find(boundary);

	while (pos != std::string::npos)
	{
		++count;
		pos = request_body.find(boundary, pos + boundary.size());
	}

	return (count);
}

Mime classifyMime(const std::string &request_body)
{
	// 1) Find content type
	const std::string key = "Content-Type: ";
	size_t pos = request_body.find(key);
	if (pos == std::string::npos)
	{
		Logger::info("RequestHandler::composeMultiPartData::classifyMime() → Did not find '" + key + "'");
		return (Mime(""));
	}

	// 3) Start of value
	size_t value_start = pos + key.size();
	size_t value_end = request_body.find("\r\n", value_start);

	// 4) Find end of value (up to next CRLF)
	if (value_end == std::string::npos)
	{
		Logger::info("RequestHandler::composeMultiPartData::classifyMime() → Did not find 'value_end'");
		return (Mime(""));
	}

	return (request_body.substr(value_start, (value_end - value_start)));
}

const std::string subString(const std::string &request_body, const std::string &key, const std::string &end)
{
	size_t pos = request_body.find(key);
	if (pos == std::string::npos)
	{
		Logger::info("RequestHandler::composeMultiPartData::subString() → Did not find '" + key + "'");
		return ("");
	}
	size_t value_start = pos + key.size();

	size_t value_end = request_body.find(end, value_start);
	if (value_end == std::string::npos)
	{
		Logger::info("RequestHandler::composeMultiPartData::subString() → Did not find 'value_end'");
		return ("");
	}
	return (request_body.substr(value_start, (value_end - value_start)));
}

const MultipartFile RequestHandler::composeMultiPartData(const HttpRequest &request) const
{
	// 1) Count boundaries
	const size_t boundaries = countBoundaries(request.body, request.content_type.getBoundary());
	if (boundaries < 1)
	{
		Logger::error("RequestHandler::composeMultiPartData() → Found less than 2 boundaries");
		return (MultipartFile());
	}

	// 2) Try to find title
	std::string title = subString(request.body, "name=\"title\"\r\n\r\n", "\r\n");
	if (title.empty())
	{
		// Fallback to orignal filename
		title = subString(request.body, "filename=\"", "\"");
	}
	if (title.empty())
	{
		Logger::error("RequestHandler::composeMultiPartData() → Could not find document title");
		return (MultipartFile());
	}

	// 3) Classify Mime
	Mime mime = Mime(subString(request.body, "Content-Type: ", "\r\n"));

	// 4) Extract raw body data
	const std::string raw_body = subString(request.body, "\r\n\r\n", "\r\n--" + request.content_type.getBoundary());

	const MultipartFile multipartFile(raw_body, title, mime);
	return (multipartFile);
}
