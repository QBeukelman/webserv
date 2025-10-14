/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ContentType.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/24 10:04:55 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/14 01:55:11 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTENTTYPE_HPP
#define CONTENTTYPE_HPP

#include "http/models/Mime.hpp"

#include <map>
#include <string>

enum ContentTypeKind
{
	UNKONOWN,
	URLENCODED,
	MULTIPART,
	PLAIN_TEXT
};

class ContentType
{
  private:
	ContentTypeKind type;
	Mime mime;
	std::string boundary;

	ContentTypeKind classifyContentType(const std::string &raw);

  public:
	ContentType();
	ContentType(const std::string &raw);

	ContentTypeKind getType(void) const;

	const Mime getMime(void) const;

	const std::string getBoundary(void) const;
	void setBoundary(const std::string newBoundary);
};

std::string toStringContentType(const ContentTypeKind &type);

#endif
