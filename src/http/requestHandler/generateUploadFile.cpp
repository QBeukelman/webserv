/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   generateUploadFile.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/19 11:42:45 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/23 13:15:48 by quentinbeuk   ########   odam.nl         */
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
	for (int i = 0; i < width; i++)
		oss << "0";
	oss << value;
	return (oss.str());
}

static std::string paddedName(int i)
{
	const unsigned int width = (numDigits(MAX_UPLOAD_FILES) - numDigits(i));
	return (zeroPad(i, width));
}

static const std::string newFileName(const std::string &upload_dir)
{
	// Normalize directory
	std::string dir = upload_dir;
	if (dir.empty() == false && dir[dir.size() - 1] != '/')
		dir += '/';

	// Start at 1 to produce 00001 as the first name
	unsigned int i = 0;
	while (i < MAX_UPLOAD_FILES)
	{
		std::string name = "upload_" + paddedName(i) + ".txt";
		std::string candidate = dir + name;

		if (access(candidate.c_str(), F_OK) != 0)
		{
			// Not found, take it
			return (candidate);
		}
		i++;
	}

	// No new names
	return (std::string());
}

/*
 * Create a new file with unique name in the given directory. Names are appended with `0`
 * .e.g. `var/www/uploads/upload_00001.txt`.
 *
 * Returns:
 * 	- An open File with `name` and `fd`.
 *  - `-1` on failure.
 *
 * Notes:
 * 	- Files have `.txt` extension.
 * 	- File names are prepended with `upload_`
 * 	- Limited to `10000` files.
 */
const File RequestHandler::generateUploadFile(const std::string &upload_dir) const
{
	const std::string newName = newFileName(upload_dir);
	if (newName.empty())
	{
		Logger::error("RequestHandler::generateUploadFile() → Exceeded allowed number of uploads");
		return (File("", -1));
	}

	const int fd = open(newName.data(), O_WRONLY | O_CREAT | O_EXCL, 0644);

	return (File(newName, fd));
}
