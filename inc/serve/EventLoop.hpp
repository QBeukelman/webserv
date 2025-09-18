/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:14:45 by qbeukelm          #+#    #+#             */
/*   Updated: 2025/09/15 11:23:39 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include "log/Logger.hpp"
#include "serve/IOPollable.hpp"

#include <map>
#include <poll.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>

class Connection; // Forward

class EventLoop
{
  private:
	std::map<int, IOPollable *> handlers; // fd → handler
	std::vector<pollfd> pfds;
	std::vector<IOPollable *> pendingClose;

	void willClosePending();

  public:
	EventLoop();

	// Getters & Setters
	std::vector<pollfd> getPollFds(void) const;

	// Registration
	void add(IOPollable *handler);
	void update(IOPollable *handler);
	void remove(int fd);
	void closeLater(IOPollable *handler);

	// Utils
	unsigned long nowMs() const;

	// Main loop
	void run(void);
	void stop(void);
};

#endif
