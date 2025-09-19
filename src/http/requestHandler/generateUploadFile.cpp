/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   generateUploadFile.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/19 11:42:45 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/19 12:03:38 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

static int numDigits(int i)
{
	int d = 1;

	while (i >= 10)
	{
		i /= 10;
		++d;
	}
	return (d);
}

static std::string zeroPad(unsigned int value, unsigned int width)
{
	std::ostringstream oss;
	oss << value;
	std::string s = oss.str();
	if (s.size() < width)
		s.insert(0, width - s.size(), '0');
	return (s);
}

static std::string paddedName(int i)
{
	const unsigned int width = numDigits(MAX_UPLOAD_FILES - 1);
	return (zeroPad(static_cast<unsigned int>(i), width));
}

// TODO: RequestHandler::generateUploadFile -> Generate a file
const std::string RequestHandler::generateUploadFile(const std::string &upload_dir) const
{
	// Normalize directory
	std::string dir = upload_dir;
	if (dir.empty() == false && dir[dir.size() - 1] != '/')
		dir += '/';

	const unsigned int width = numDigits(MAX_UPLOAD_FILES - 1);

	// Start at 1 to produce 00001 as the first name
	int i = 0;
	while (i < MAX_UPLOAD_FILES)
	{
		std::string name = "upload_" + zeroPad(i, width) + ".txt";
		std::string candidate = dir + name;

		if (access(candidate.c_str(), F_OK) != 0)
		{
			// Not found, take it
			return (candidate);
		}
	}

	// No new names
	return (std::string());
}
