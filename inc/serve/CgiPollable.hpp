/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiPollable.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/06 11:54:18 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/12 21:23:00 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CgiPollable_HPP
#define CgiPollable_HPP

#include "config/Location.hpp"
#include "config/models/CgiConfig.hpp"
#include "http/models/HttpRequest.hpp"
#include "http/models/HttpResponse.hpp"
#include "serve/CgiPollable.hpp"
#include "serve/IOPollable.hpp"
#include "serve/cgi/cgiHelpers.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

class EventLoop;
class Connection;

class CgiPollable
{
  private:
	void registerPollables(bool);
	void unregisterPollables();
	void maybeFinish();
	void makeCgiResponse();
	bool extractAndWaitChild(int &status);
	void terminateChild();

	void removeStdinFromLoop();
	void removeStdoutFromLoop();

  private:
	EventLoop *loop;
	Connection *ownerConnection;
	const HttpRequest &request;
	const Location &location;
	const CgiConfig &cgiConfig;

	// Child / Pipes
	pid_t pid;
	int stdin_fd;
	int stdout_fd;
	int stdout_reg_fd = -1;
	int stdin_reg_fd = -1;

	// Buffer
	std::string in_buf;
	std::string out_buf;

	// IO
	class CgiStdoutPollable;
	class CgiStdinPollable;
	std::unique_ptr<CgiStdoutPollable> stdout_poll;
	std::unique_ptr<CgiStdinPollable> stdin_poll;

	// States
	bool stdout_closed = false;
	bool stdin_closed = false;

  public:
	CgiPollable(EventLoop *loop, Connection *ownerConnection, const HttpRequest &request, const Location &location,
				const CgiConfig &cgiConfig);

	~CgiPollable();

	bool start();

	void readFromChild(); // stdout → outBuf
	void writeToChild();  // inBuf → stdin
	void stdoutHangup(short revents);
	void stdinHangup(short revents);
	void clientAborted();
};

// I/O
// ____________________________________________________________________________
class CgiPollable::CgiStdoutPollable : public IOPollable
{
  public:
	explicit CgiStdoutPollable(CgiPollable &newOwnerProcess);
	int fd() const override;
	short interest() const override;
	void onReadable() override;
	void onWritable() override;
	void onHangupOrError(short revents) override;

  private:
	CgiPollable &ownerProcess;
};

class CgiPollable::CgiStdinPollable : public IOPollable
{
  public:
	explicit CgiStdinPollable(CgiPollable &newOwnerProcess);
	int fd() const override;
	short interest() const override;
	void onReadable() override;
	void onWritable() override;
	void onHangupOrError(short revents) override;

  private:
	CgiPollable &ownerProcess;
};

#endif
