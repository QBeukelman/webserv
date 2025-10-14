/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   TokenStream.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: hein <hein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 22:45:34 by hein          #+#    #+#                 */
/*   Updated: 2025/10/14 01:39:53 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENSTREAM_HPP
#define TOKENSTREAM_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class EndOfFileException : public std::exception
{
  public:
	const char *what() const noexcept override
	{
		return "End of file reached";
	}
};

class TokenStream
{
  private:
	std::vector<std::string> tokenStream;
	std::ifstream file;
	int lineCount;
	unsigned int tokenIndex;

	void nextValidLine(std::stringstream &ss);

  public:
	TokenStream();

	std::string next(void);
	void expectOpenBracket(const std::string &expected);
	bool awaitClosingBracket(const std::string &currentToken);
	void openFile(const std::string &path);
	void setNextTokenStream(void);
	const std::string getLineCount(void);
	const std::string &getCurrentToken(void);
	std::size_t validateMinimumArguments(int n);
	void validateExpectedArguments(unsigned int n);
	void removeValidSemicolon(void);

	void throwError(const std::string &error);
};

#endif