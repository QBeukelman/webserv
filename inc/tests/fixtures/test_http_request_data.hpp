/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_http_request_data.hpp                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/26 13:59:07 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/05 13:50:49 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HTTP_REQUEST_DATA_HPP
#define TEST_HTTP_REQUEST_DATA_HPP

// Simple GET (no body)
static const char HTTP_REQUEST_GET[] = "GET / HTTP/1.1\r\n"
									   "Host: a\r\n"
									   "\r\n";

// POST with body
static const char HTTP_REQUEST_POST_CONTENT_LEN[] = "POST /submit-form?debug=true HTTP/1.1\r\n"
													"Host: www.example.com\r\n"
													"User-Agent: curl/7.68.0\r\n"
													"Content-Type: application/x-www-form-urlencoded\r\n"
													"Content-Length: 26\r\n"
													"\r\n"
													"name=Alice&age=30&city=AMS";

// POST with arge body
static const char HTTP_REQUEST_POST_CHUNKED[] = "POST /submit-form?debug=true HTTP/1.1\r\n"
												"Host: www.example.com\r\n"
												"User-Agent: curl/7.68.0\r\n"
												"Content-Type: application/x-www-form-urlencoded\r\n"
												"Transfer-Encoding: chunked\r\n"
												"\r\n"
												"7\r\n"
												"Mozilla\r\n"
												"9\r\n"
												"Developer\r\n"
												"7\r\n"
												"Network\r\n"
												"0\r\n"
												"\r\n";

// POST with body
static const char HTTP_REQUEST_POST_NO_CRLF[] = "POST /submit-form?debug=true HTTP/1.1\r\n"
												"Host: www.example.com\r\n"
												"User-Agent: curl/7.68.0\r\n"
												"Content-Type: application/x-www-form-urlencoded\r\n"
												"Content-Length: 26\r\n"
												"\r"
												"name=Alice&age=30&city=AMS";

#endif
