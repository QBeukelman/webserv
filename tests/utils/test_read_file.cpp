/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_read_file.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:56:02 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 12:26:39 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests/utils/test_read_file.hpp"

inline std::string read_file(const std::string &path)
{
	std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary);
	if (!ifs)
	{
		throw std::runtime_error("Could not open file: " + path);
	}
	std::ostringstream oss;
	oss << ifs.rdbuf();
	return (oss.str());
}
