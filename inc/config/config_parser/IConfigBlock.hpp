/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IConfigBlock.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/19 09:45:40 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/19 10:01:08 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICONFIGBLOCK_HPP
#define ICONFIGBLOCK_HPP

#include <string>

class IConfigBlock
{
  public:
	virtual ~IConfigBlock()
	{
	}

	// Root
	virtual void setRoot(const std::string &root) = 0;
	virtual std::string getRoot() const = 0;

	// Index files
	virtual bool addIndexFile(const std::string &index) = 0;

	// Directive bitmask helpers
	virtual void markDirective(unsigned int directive) = 0;
	virtual bool hasDirective(unsigned int directive) = 0;
	virtual bool requiredDirectives(unsigned int directive) = 0;
};

#endif
