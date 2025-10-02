/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generateAutoIndexResponse.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:52:50 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/10/02 10:36:39 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

static std::vector<std::string> generateDirectoryListing(const std::string &current_directory)
{
	std::vector<std::string> entries;

	DIR *directory = opendir(current_directory.c_str());
	if (directory == NULL)
		throw std::runtime_error("Could not open directory: " + current_directory);

	struct dirent *entry;
	while ((entry = readdir(directory)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;

		// Check if it is a directory
		std::string fullPath = current_directory + "/" + name;
		struct stat st;
		if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
		{
			name += "/";
		}

		entries.push_back(name);
	}
	closedir(directory);

	// Alphabetical order
	std::sort(entries.begin(), entries.end());
	return (entries);
}

static std::string makeHtmlBody(const std::string &htmlHeader, const std::vector<std::string> entries,
								const std::string &path)
{

	std::ostringstream oss;

	// === head ===
	oss << htmlHeader;

	// === body ===
	oss << "<body>";

	oss << "<div class=\"spaced-section\">";

	oss << "<h1>" << "Auto Index: " << "</h1>";

	oss << "<p>" << path << "</p>\n";

	for (const auto &e : entries)
	{
		oss << "<a href=\"" << e << "\" class=\"button\">" << e << "</a>\n";
	}

	oss << "<div class=\"center-btn\">";
	oss << "<a href=\"../../index.html\" class=\"back-home-button\">← Back to Home</a>";
	oss << "</div>";

	oss << "</div>";

	oss << "</body>";
	oss << "</html>";

	return (oss.str());
}

HttpResponse RequestHandler::generateAutoIndexResponse(const std::string &file_path) const
{
	std::vector<std::string> entries;

	try
	{
		entries = generateDirectoryListing(file_path);
	}
	catch (std::exception &e)
	{
		// TODO: Make error
	}

	HttpResponse response;
	response.httpStatus = HttpStatus::STATUS_OK; // 200

	response.headers[KEY_CONTENT_TYPE] = "text/html; charset=UTF-8";
	response.body = makeHtmlBody(makeHtmlPageHeader(), entries, file_path);
	response.headers[KEY_CONTENT_LENGTH] = std::to_string(response.body.size());

	return (response);
}
