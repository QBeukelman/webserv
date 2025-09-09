/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   EventLoop.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 12:49:07 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/09 17:11:25 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/EventLoop.hpp"

// CONSTRUCTORS
// ____________________________________________________________________________
EventLoop::EventLoop() : handlers(std::map<int, IOPollable *>())
{
	Logger::info("EventLoop::EventLoop()");
}

std::vector<pollfd> EventLoop::getPollFds(void) const
{
	return (this->pfds);
}

// PUBLIC
// ____________________________________________________________________________
/*
 * Add an fd and Client pair:
 *
 * map <`fd`, `IOPollable`>
 */
void EventLoop::add(IOPollable *handler)
{
	int fd = handler->fd();
	handlers[fd] = handler;

	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = handler->interest();
	pfd.revents = 0;
	pfds.push_back(pfd);
}

/*
 * This is the function that calls `poll()`.
 */
void EventLoop::run(void)
{
	Logger::info("EventLoop → STARTED");

	while (true)
	{
		int n = ::poll(&pfds[0], pfds.size(), -1);
		if (n == 0)
		{
			Logger::error("EventLoop::run() → Poll timed out");
			continue;
		}
		if (n < 0)
		{
			Logger::error("EventLoop::run() → Poll returned error");
			continue;
		}

		for (size_t i = 0; i < pfds.size(); i++)
		{
			short re = pfds[i].revents;
			if (!re)
				continue;

			IOPollable *h = handlers[pfds[i].fd];
			if (re & POLLIN)
			{
				// TODO: run() → data may be read without blocking.
				h->onReadable();
			}
			if (re & POLLOUT)
			{
				// TODO: run() → data may be written without blocking.
				h->onWritable();
			}
			if (re & (POLLERR | POLLHUP | POLLNVAL))
			{
				// TODO: run() → error or handup.
				h->onHangupOrError(re);
			}
		}
	}
}

void EventLoop::stop(void)
{
	Logger::info("EventLoop → STOPPED");
}
