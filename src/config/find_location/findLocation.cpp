/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   findLocation.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 14:04:15 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/03 14:53:37 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/ServerConfig.hpp"

static bool has_segment_boundry_match(const std::string &path, const std::string &prefix)
{
	if (path.compare(0, prefix.size(), prefix) != 0)
		return (false);
	if (prefix.empty())
		return (false);

	// Ensure that we do not match `/adm` for `/admin`
	if (prefix[prefix.size() - 1] == '/')
		return (true);

	// Either exact length match, or next char in path is a '/'
	if (path.size() == prefix.size())
		return (true);
	return (path.size() > prefix.size() && path[prefix.size()] == '/');
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
const Location *ServerConfig::findLocation(std::string requestPath) const
{
	const Location *best = NULL;
	size_t best_len = 0;

	for (const auto &l : locations)
	{
		if (has_segment_boundry_match(requestPath, l.path_prefix))
		{
			if (l.path_prefix.size() > best_len)
			{
				best = &l;
				best_len = l.path_prefix.size();
			}
		}
	}
	return (best);
}
