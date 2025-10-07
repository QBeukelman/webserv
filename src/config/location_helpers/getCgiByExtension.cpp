/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   getCgiByExtension.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/27 14:18:03 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/07 15:26:18 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/Location.hpp"

const std::string Location::getFileExtenstion(const std::string &request_path) const
{
	const std::string::size_type dot_pos = request_path.find_last_of('.');
	if (dot_pos == std::string::npos || dot_pos == request_path.size() - 1)
		return ("");
	return (request_path.substr(dot_pos));
}

// CGI
// ____________________________________________________________________________
std::optional<CgiConfig> Location::getCgiByExtension(const std::string &requestPath) const
{
	const std::string extenstion = getFileExtenstion(requestPath);

	if (extenstion.empty())
	{
		return (std::nullopt);
	}

	std::map<std::string, CgiConfig>::const_iterator it = cgis.find(extenstion);
	if (it == cgis.end())
		return (std::nullopt); // Not found

	return (it->second); // Found
}
