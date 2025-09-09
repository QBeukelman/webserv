/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   EventLoop.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 12:14:45 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/09 17:21:13 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include "log/Logger.hpp"
#include "serve/Connection.hpp"

#include <poll.h>

class EventLoop
{
  private:
	std::map<int, IOPollable *> handlers; // fd → handler
	std::vector<pollfd> pfds;

  public:
	EventLoop();

	// Getters & Setters
	std::vector<pollfd> getPollFds(void) const;

	// Registration
	void add(IOPollable *handler);
	void addListner(int fd);
	void remove(int fd);

	// Main loop
	void run(void);
	void stop(void);
};

#endif
