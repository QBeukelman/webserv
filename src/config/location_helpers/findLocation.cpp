/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   findLocation.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 14:04:15 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/28 10:11:07 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"

static bool has_segment_boundary_match(const std::string &path, const std::string &prefix)
{
	if (prefix.empty())
		return (false);
	if (prefix == "/")
		return (true);

	if (path.compare(0, prefix.size(), prefix) != 0)
		return (false);

	if (path.size() == prefix.size())
		return (true);

	return (path[prefix.size()] == '/');
}

// FIND LOCATION
// ____________________________________________________________________________
/*
 * Choose the longest path prefex, whos boundries aligh with requestPath.
 * The best match is the one with the longest valid prefix match.
 *
 * GET /images/icons/logo.png
 * 		- `/`→ Yes (everything matches `/`).
 * 		- `/images/` → Yes.
 * 		- `/images/icons/` → Best match.
 *
 * Notes:
 * 		- May return `NULL` → caller must handle fallback.
 */
Location Server::findLocation(const std::string &rawPath) const
{

	// 1) Normalize path (Ensure leading `/`)
	std::string normalized_path = rawPath;
	if (normalized_path.empty() || normalized_path[0] != '/')
		normalized_path.insert(normalized_path.begin(), '/');

	// 2) Longest prefix match
	const Location *best = NULL;
	size_t best_len = 0;

	std::vector<Location>::const_iterator it = locations.begin();
	while (it != locations.end())
	{
		std::cout << "Searching location: " << it->path_prefix << " path: " << normalized_path << std::endl;

		if (has_segment_boundary_match(normalized_path, it->path_prefix))
		{
			if (it->path_prefix.size() > best_len)
			{
				best = &(*it);
				best_len = it->path_prefix.size();
			}
		}
		it++;
	}

	if (best)
		return (*best);
	throw LocationNotFoundException();
}
