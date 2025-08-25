/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_test_file.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:53:00 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/25 12:23:05 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_TEST_FILE_HPP
#define READ_TEST_FILE_HPP

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

std::string read_file(const std::string &path);

#endif
