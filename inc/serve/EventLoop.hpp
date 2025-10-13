/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   EventLoop.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 12:14:45 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/13 10:32:29 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include "log/Logger.hpp"
#include "serve/IOPollable.hpp"

#include <chrono>
#include <climits>
#include <iostream>
#include <map>
#include <poll.h>
#include <set>
#include <sys/time.h>
#include <unistd.h>
#include <unordered_set>
#include <vector>

class Connection; // Forward

class EventLoop
{
  private:
	std::map<int, IOPollable *> handlers; // fd → handler
	std::vector<pollfd> pfds;
	std::vector<IOPollable *> pendingClose;
	std::unordered_set<int> pending_close_fds;

	void willClosePending();
	int computePollTimeoutMs() const;
	size_t findPfdIndex(int fd) const;

  public:
	EventLoop();

	// Getters & Setters
	std::vector<pollfd> getPollFds(void) const;
	std::vector<IOPollable *> getPendingClose(void) const;
	const std::unordered_set<int> &getPendingCloseFds() const;

	// Registration
	void add(IOPollable *handler);
	void update(IOPollable *handler);
	void remove(int fd);
	void remove(IOPollable *handler);
	void closeLater(IOPollable *handler);

	// Main loop
	void run(void);
	void checkTimeouts(void);
	void stop(void);
};

#endif
