/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:13:04 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/08/19 13:35:11 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	*Parsing checklist (must-haves)

		- Split headers/body at first \r\n\r\n. Reject if headers exceed sane limits.

	*Start line: METHOD SP TARGET SP HTTP/1.1\r\n

		- Validate method (GET/POST/DELETE at minimum).
		- Extract path and query from target.
		- Version must be HTTP/1.1.

	*Headers:

		- Case-insensitive keys; trim spaces.
		- Require Host (HTTP/1.1).
		- Determine keep_alive (default true unless Connection: close).

	*Body:

		- If Transfer-Encoding: chunked → decode chunks until size 0.
		- Else if Content-Length → read exactly that many bytes.
		- Enforce client_max_body_size → 413 if exceeded.

	*Common errors:

		- 400 bad syntax/invalid header format/missing Host.
		- 405 method not allowed (later in handler per location).
		- 411 length required (body without length & not chunked).
		- 414 URI too long.
		- 501 method not implemented.
*/

// ! Example
/*
POST /upload HTTP/1.1\r\n
Host: localhost:8080\r\n
Content-Type: text/plain\r\n
Content-Length: 11\r\n
\r\n
Hello World
*/

#include "http/Request.hpp"

// bool	Request::parse(const std::string &raw) {
	
// }


// HttpResponse	Request::execute() const {
	
// 	// 1. Parse
	
	
// }

