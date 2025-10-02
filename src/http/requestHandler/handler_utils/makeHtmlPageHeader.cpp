/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makeHtmlPageHeader.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 10:14:37 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/10/02 10:17:37 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "http/RequestHandler.hpp"

std::string RequestHandler::makeHtmlPageHeader(void) const
{
	std::ostringstream oss;

	// === head ===
	oss << "<!doctype html>\n";
	oss << "<html lang=\"en\">\n";

	oss << "<head>\n";
	oss << "<meta charset=\"utf-8\">\n";
	oss << "<title>Webserv</title>\n";

	oss << "<link href=\"https://fonts.googleapis.com/css2?family=Special+Gothic+Expanded+One&display=swap\" "
		   "rel=\"stylesheet\">";
	oss << "<link href=\"https://fonts.googleapis.com/css2?family=Lora:wght@400;500;700&display=swap\" "
		   "rel=\"stylesheet\">";
	oss << "<link href=\"https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&display=swap\" "
		   "rel=\"stylesheet\">";

	oss << "<link rel=\"stylesheet\" href=\"/website/styles/style.css\">";

	oss << "</head>";

	return (oss.str());
}
