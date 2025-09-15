/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TokenStream.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 23:05:56 by hein          #+#    #+#                 */
/*   Updated: 2025/09/15 11:29:20 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config/config_parser/TokenStream.hpp"

#include <cctype>
#include <fstream>
#include <sstream>

TokenStream::TokenStream()
{
	lineCount = 0;
	tokenIndex = 0;
}

static void trimWhitespace(std::string &line)
{
	size_t start = line.find_first_not_of(" \t\r\n");

	if (start == std::string::npos)
	{
		line.clear();
		return;
	}

	if (start > 0)
	{
		line.erase(0, start);
	}

	size_t end = line.find_last_not_of(" \t\r\n");

	if (end < (line.length()))
	{
		line.erase(end + 1);
	}
}

void TokenStream::nextValidLine(std::stringstream &ss)
{
	std::string line;

	while (true)
	{
		if (!std::getline(file, line))
		{
			if (file.eof())
				throw EndOfFileException();
			else
				throw std::runtime_error("An error occured during reading of the file.");
		}

		++lineCount;
		trimWhitespace(line);

		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		ss.clear();
		ss.str(line);

		return;
	}
}

void TokenStream::setNextTokenStream(void)
{
	std::stringstream ss;
	std::string token;

	nextValidLine(ss);

	tokenStream.clear();

	while (ss >> token)
	{
		tokenStream.push_back(token);
		token.clear();
	}

	tokenIndex = 0;
}

std::string TokenStream::next(void)
{
	++tokenIndex;

	if (tokenIndex >= tokenStream.size())
	{
		setNextTokenStream();
	}

	return tokenStream[tokenIndex];
}

void TokenStream::expectOpenBracket(const std::string &currentToken)
{
	if (currentToken != "{")
	{
		throw std::runtime_error("Error on line " + std::to_string(lineCount) + ". Expected [ { ] but found [ " +
								 currentToken + " ]");
	}
	else if ((tokenIndex != tokenStream.size() - 1))
	{
		std::string junk = next();
		if (junk != "}")
		{
			throw std::runtime_error("Error on line " + std::to_string(lineCount) + ". Unexpected input [ " + junk +
									 " ] after open bracket was found");
		}
	}
}

bool TokenStream::awaitClosingBracket(const std::string &currentToken)
{
	return currentToken == "}";
}

void TokenStream::openFile(const std::string &path)
{
	file.open(path.c_str());

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open config file: " + path);
	}
}

const std::string TokenStream::getLineCount(void)
{
	return std::to_string(lineCount);
}

const std::string &TokenStream::getCurrentToken(void)
{
	return tokenStream[tokenIndex];
}

void TokenStream::removeValidSemicolon(void)
{
	std::string &token = tokenStream.back();

	if (!token.empty() && (token.back() == ';'))
	{
		token.pop_back();
	}
	else
	{
		throw std::runtime_error("Parsing Error on line " + getLineCount() +
								 " Expecting semicolon [ ; ] on last argument");
	}
}

std::size_t TokenStream::validateMinimumArguments(int n)
{
	std::size_t argumentCount = tokenStream.size() - 1;

	if (argumentCount < n)
	{
		throw std::runtime_error("Parsing Error on line " + getLineCount() + " Wrong amount of arguments");
	}
	return (argumentCount);
}

void TokenStream::validateExpectedArguments(int n)
{
	if (tokenStream.size() - 1 != n)
	{
		throw std::runtime_error("Parsing Error on line " + getLineCount() + " Wrong amount of arguments");
	}
}

void TokenStream::throwError(const std::string &error)
{
	throw std::runtime_error("Parsing Error on line " + getLineCount() + ". " + error);
}
