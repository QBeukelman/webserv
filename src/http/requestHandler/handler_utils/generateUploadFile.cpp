/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   generateUploadFile.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/19 11:42:45 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/03 14:58:28 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

/*
 * Returns:
 * 	- An open File with `name` and `fd`.
 *  - `-1` on failure.
 */
const File RequestHandler::generateUploadFile(const std::string &upload_dir, const std::string file_name) const
{
	const std::string file_path = upload_dir + "/" + file_name;

	const int fd = open(file_path.data(), O_WRONLY | O_CREAT, 0644);
	return (File(file_path, fd));
}
