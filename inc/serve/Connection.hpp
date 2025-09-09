/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 14:27:52 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/09/09 17:22:35 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "config/Server.hpp"
#include "http/models/ParseContext.hpp"
#include "log/Logger.hpp"
#include "serve/IOPollable.hpp"

#include <string>

/*
 * A `Connection` represents an active TCP session with a single client.
 * It holds:
 *
 * 		- Client fd's from `accept()`.
 * 		- Input buffer and `ParseContext()`.
 * 		- Flags like `keep-alive` and timeouts, etc.
 */
class Connection : public IOPollable
{
  private:
	int fd_;
	const Server *server;
	ParseContext parse;
	std::string inBuf;
	std::string outBuf;
	bool keepAlive;
	unsigned long lastActivityMs;

  public:
	inline Connection(int clientFd);

	// IOPollable
	int fd();
	short interest() const;
	void onReadable();
	void onWritable();
	void onHangupOrError(short revents);
};

#endif
