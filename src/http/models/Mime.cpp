/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Mime.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/24 10:27:47 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/24 11:30:59 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http/models/Mime.hpp"

static const std::map<std::string, std::string> mimeTypes{
	{".html", "text/html"}, {".css", "text/css"},  {".js", "application/javascript"}, {".png", "image/png"},
	{".jpg", "image/jpeg"}, {".gif", "image/gif"}, {".json", "application/json"},	  {".pdf", "application/pdf"},
};

/*
 * Given `application/pdf` returns extension `.pdf`.
 */
std::string Mime::getExtension(const std::string &type_string) const
{
	std::map<std::string, std::string>::const_iterator it = mimeTypes.find(type_string);
	if (it != mimeTypes.end())
		return (it->first);
	return ("");
}

/*
 * Given extension `.pdf` returns file type string `application/pdf`.
 */
std::string Mime::getTypeString(const std::string &extension) const
{
	std::map<std::string, std::string>::const_iterator it = mimeTypes.find(extension);
	if (it != mimeTypes.end())
		return (it->second);
	return ("");
}
