/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiProcess.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/06 11:54:18 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/12 21:23:00 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIPROCESS_HPP
#define CGIPROCESS_HPP

#include "config/Location.hpp"
#include "config/models/CgiConfig.hpp"
#include "http/models/HttpRequest.hpp"
#include "http/models/HttpResponse.hpp"
#include "serve/CgiProcess.hpp"
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

class CgiProcess
{
  public:
	CgiProcess(EventLoop *loop, Connection *ownerConnection, const HttpRequest &request, const Location &location,
			   const CgiConfig &cgiConfig);

	~CgiProcess();

	bool start();

	void readFromChild(); // stdout → outBuf
	void writeToChild();  // inBuf → stdin
	void stdoutHangup(short revents);
	void stdinHangup(short revents);
	void clientAborted();

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
};

// I/O
// ____________________________________________________________________________
class CgiProcess::CgiStdoutPollable : public IOPollable
{
  public:
	explicit CgiStdoutPollable(CgiProcess &newOwnerProcess);
	int fd() const override;
	short interest() const override;
	void onReadable() override;
	void onWritable() override;
	void onHangupOrError(short revents) override;

  private:
	CgiProcess &ownerProcess;
};

class CgiProcess::CgiStdinPollable : public IOPollable
{
  public:
	explicit CgiStdinPollable(CgiProcess &newOwnerProcess);
	int fd() const override;
	short interest() const override;
	void onReadable() override;
	void onWritable() override;
	void onHangupOrError(short revents) override;

  private:
	CgiProcess &ownerProcess;
};

#endif
