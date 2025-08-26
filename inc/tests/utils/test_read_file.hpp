/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_read_file.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/25 10:53:00 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/08/26 08:30:36 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_TEST_FILE_HPP
#define READ_TEST_FILE_HPP

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

std::string read_file(const std::string path);

#endif
