/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   EventLoop.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 12:49:07 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/09/18 11:36:42 by quentinbeuk   ########   odam.nl         */
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

// REGISTRATION
// ____________________________________________________________________________
/*
 * Add an fd and Client pair:
 *
 * map <`fd`, `IOPollable`>
 */
void EventLoop::add(IOPollable *h)
{
	int fd = h->fd();
	if (fd < 0)
	{
		Logger::error("EventLoop::add() → Invalid fd");
		return;
	}

	if (handlers.find(fd) != handlers.end())
	{
		Logger::error("EventLoop::add → fd already registered: " + std::to_string(fd));
		return;
	}

	handlers[fd] = h;

	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = h->interest();
	pfd.revents = 0;
	pfds.push_back(pfd);

	Logger::info("EventLoop::add → registered fd " + std::to_string(fd));
}

/*
 * Desired poll event changed. Update `pollfd.events` for fd.
 *
 * Notes:
 * 	- With `poll()`, we decide per-fd which events are needed (`POLLIN`, `POLLOUT`, ...).
 * 	- Call this after a state change (e.g. `outbuf` became non-empty → want `POLLOUT).
 */
void EventLoop::update(IOPollable *handler)
{
	const int fd = handler->fd();
	for (size_t i = 0; i < pfds.size(); i++)
	{
		if (pfds[i].fd == fd)
		{
			pfds[i].events = handler->interest();
			break;
		}
	}
}

void EventLoop::remove(int fd)
{
	for (size_t i = 0; i < pfds.size(); ++i)
	{
		if (pfds[i].fd == fd)
		{
			pfds.erase(pfds.begin() + i);
			break;
		}
	}
	handlers.erase(fd);
	::close(fd);
}

/*
 * Mark connection for closure. EventLoop will close connection after finishing current poll iteration.
 *
 * Notes:
 * 		- May not safely delete or close a connection while inside event callback
 * 		`onReadable()` or `onWritable()`. This risks invalidating array of
 * 		poll-Fds iterated over in main EventLoop.
 */
void EventLoop::closeLater(IOPollable *handler)
{
	pendingClose.push_back(handler);
}

// UTILS
// ____________________________________________________________________________
unsigned long EventLoop::nowMs() const
{
	struct timeval tv;
	::gettimeofday(&tv, NULL);

	// Convert seconds + microseconds = milliseconds
	unsigned long sec = static_cast<unsigned long>(tv.tv_sec) * 1000ul;
	unsigned long usec = static_cast<unsigned long>(tv.tv_usec) / 1000ul;
	return (sec + usec);
}

void EventLoop::willClosePending()
{
	if (pendingClose.empty())
		return;
	else
	{
		for (size_t i = 0; i < pendingClose.size(); ++i)
		{
			IOPollable *handler = pendingClose[i];
			if (!handler)
				continue;

			const int fd = handler->fd();

			// Remove from handler map
			std::map<int, IOPollable *>::iterator found = handlers.find(fd);
			if (found != handlers.end() && found->second == handler)
				handlers.erase(found);

			// Remove from pfds
			for (size_t j = 0; j < pfds.size(); ++j)
			{
				if (pfds[j].fd == fd)
				{
					pfds.erase(pfds.begin() + j);
					break;
				}
			}

			// Close fd and destroy
			::close(fd);
			delete (handler);
			pendingClose[i] = NULL;
		}
		pendingClose.clear();
	}
}

// MAIN LOOP
// ____________________________________________________________________________
/*
 * This is the function that calls `poll()`.
 */
void EventLoop::run(void)
{
	Logger::info("EventLoop → STARTED");

	while (true)
	{
		// 1) Poll
		// What if poll() → UB if empty.
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

		// 2) Dispatch events
		for (size_t i = 0; i < pfds.size(); i++)
		{
			const int fd = pfds[i].fd;
			const short re = pfds[i].revents;
			if (!re)
				continue;

			std::map<int, IOPollable *>::iterator it = handlers.find(fd);
			if (it == handlers.end() || it->second == NULL)
			{
				Logger::error("EventLoop: no handler for fd " + std::to_string(fd));
				continue;
			}
			IOPollable *h = it->second;

			if (re & (POLLERR | POLLHUP | POLLNVAL))
			{
				// TODO: run() → error or handup.
				Logger::info("EventLoop::run() → onHangupOrError()");
				h->onHangupOrError(re);
			}
			if (re & POLLIN)
			{
				// TODO: run() → data may be read without blocking.
				Logger::info("EventLoop::run() → onReadable()");
				h->onReadable();
			}
			if (re & POLLOUT)
			{
				// TODO: run() → data may be written without blocking.
				Logger::info("EventLoop::run() → onWritable()");
				h->onWritable();
			}
		}

		// 3) Cleanup defferred closes
		willClosePending();
	}
}

void EventLoop::stop(void)
{
	// TODO: stop() → STOPPED
	Logger::info("EventLoop → STOPPED");
}
