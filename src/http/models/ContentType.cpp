/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ContentType.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/24 10:06:33 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/24 11:29:51 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/ContentType.hpp"

ContentType::ContentType()
{
}

ContentType::ContentType(const std::string &raw)
{
	this->type = classifyContentType(raw);
}

ContentTypeKind ContentType::classifyContentType(const std::string &raw)
{
	if (raw.find("multipart/form-data") == 0)
		return (MULTIPART);
	if (raw == "application/x-www-form-urlencoded")
		return (URLENCODED);
	if (raw == "text/plain")
		return (PLAIN_TEXT);
	return (UNKONOWN);
}

const ContentTypeKind ContentType::getType(void) const
{
	return (this->type);
}

const Mime ContentType::getMime(void) const
{
	return (this->mime);
}

const std::string ContentType::getBoundary(void) const
{
	return (this->boundary);
}

void ContentType::setBoundary(const std::string newBoundary)
{
	this->boundary = newBoundary;
}

std::string toStringContentType(const ContentTypeKind &type)
{
	switch (type)
	{
	case MULTIPART:
		return ("Multi-Part");
	case URLENCODED:
		return ("URL-Encoded");
	case PLAIN_TEXT:
		return ("Plain-Text");
	default:
		return ("Unknown");
	}
	return ("Unknown");
}
