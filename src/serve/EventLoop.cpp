/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   EventLoop.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/08 12:49:07 by qbeukelm      #+#    #+#                 */
/*   Updated: 2025/10/13 16:18:55 by quentinbeuk   ########   odam.nl         */
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

std::vector<IOPollable *> EventLoop::getPendingClose(void) const
{
	return (this->pendingClose);
}

const std::unordered_set<int> &EventLoop::getPendingCloseFds() const
{
	return (pending_close_fds);
}

// TIME OUT
// ____________________________________________________________________________
int EventLoop::computePollTimeoutMs() const
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	int min_ms = INT_MAX;

	for (std::map<int, IOPollable *>::const_iterator it = handlers.begin(); it != handlers.end(); ++it)
	{
		const IOPollable *h = it->second;
		int ms = h->timeBudgetMs(now); // INT_MAX is "no timer"
		if (ms < min_ms)
			min_ms = ms;
	}

	if (min_ms == INT_MAX)
		return (-1); // Block until an event arrives

	if (min_ms < 0)
		min_ms = 0; // Fire

	return (min_ms);
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
	Logger::info("EventLoop::add() → Adding fd: " + std::to_string(h->fd()));
	if (!h)
		return;
	const int fd = h->fd();
	const short ev = h->interest();
	if (fd < 0 || ev == 0)
		return;

	if (handlers.count(fd))
	{
		handlers[fd] = h;
		size_t idx = findPfdIndex(fd);
		if (idx != static_cast<size_t>(-1))
			pfds[idx].events = ev;
		else
			pfds.push_back(pollfd{fd, ev, 0});
		return;
	}

	handlers[fd] = h;
	pfds.push_back(pollfd{fd, ev, 0});
}

/*
 * Desired poll event changed. Update `pollfd.events` for fd.
 *
 * Notes:
 * 	- With `poll()`, we decide per-fd which events are needed (`POLLIN`, `POLLOUT`, ...).
 * 	- Use after a state change (e.g. `outbuf` became non-empty → want `POLLOUT).
 */
void EventLoop::update(IOPollable *handler)
{
	if (!handler)
		return;
	int oldFd = -1;
	for (std::map<int, IOPollable *>::iterator it = handlers.begin(); it != handlers.end(); ++it)
	{
		if (it->second == handler)
		{
			oldFd = it->first;
			break;
		}
	}

	const int newFd = handler->fd();
	const short ev = handler->interest();

	if (oldFd == -1)
	{
		if (newFd >= 0 && ev != 0)
			add(handler);
		return;
	}

	if (ev == 0)
	{
		remove(oldFd);
		return;
	}
	if (newFd != oldFd)
	{
		remove(oldFd);
		if (newFd >= 0)
			add(handler);
		return;
	}

	size_t idx = findPfdIndex(oldFd);
	if (idx != static_cast<size_t>(-1))
		pfds[idx].events = ev;
}

void EventLoop::remove(int fd)
{
	handlers.erase(fd);

	size_t idx = findPfdIndex(fd);
	if (idx != static_cast<size_t>(-1))
	{
		size_t last = pfds.size() - 1;
		if (idx != last)
			std::swap(pfds[idx], pfds[last]);
		pfds.pop_back();
	}
}

void EventLoop::remove(IOPollable *handler)
{
	if (!handler)
		return;
	remove(handler->fd());
}

size_t EventLoop::findPfdIndex(int fd) const
{
	for (size_t i = 0; i < pfds.size(); ++i)
	{
		if (pfds[i].fd == fd)
			return (i);
	}
	return (static_cast<size_t>(-1));
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
	if (!handler)
		return;
	pendingClose.push_back(handler);
	pending_close_fds.insert(handler->fd());

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
			pendingClose[i] = NULL;
		}
		pendingClose.clear();
	}
}

// UTILS
// ____________________________________________________________________________
void EventLoop::willClosePending()
{
	if (pending_close_fds.empty())
		return;

	for (int fd : pending_close_fds)
	{
		remove(fd);
	}
	pending_close_fds.clear();
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
		// 1) Nothing to poll
		if (pfds.empty())
		{
			checkTimeouts();
			willClosePending();
			continue;
		}

		// 2) Poll
		int timeout_ms = computePollTimeoutMs();
		int n = ::poll(&pfds[0], pfds.size(), timeout_ms);
		if (n == 0)
		{
			checkTimeouts();
			willClosePending();
			continue;
		}
		if (n < 0)
			continue;

		// 3) Dispatch fds that with events
		std::vector<pollfd> copy = pfds;
		int processed = 0;
		for (size_t i = 0; i < copy.size() && processed < n;)
		{
			pollfd entry = copy[i];
			if (entry.revents == 0)
			{
				// No entry
				++i;
				continue;
			}
			++processed;

			// Find handler
			auto it = handlers.find(entry.fd);
			if (it == handlers.end())
				continue;
			IOPollable *h = it->second;

			// Skip closing
			const auto &pending = getPendingCloseFds();
			if (pending.count(entry.fd))
			{
				++i;
				continue;
			}

			// Dispatch
			short re = entry.revents;
			if (re & POLLIN)
			{
				Logger::info("EventLoop::run() → onReadable()");
				h->onReadable();
			}
			if (re & POLLOUT)
			{
				Logger::info("EventLoop::run() → onWritable()");
				h->onWritable();
			}
			if (re & (POLLERR | POLLHUP | POLLNVAL))
			{
				Logger::info("EventLoop::run() → onHangupOrError()");
				h->onHangupOrError(re);
				h->onHangupOrError(re);
				++i;
				continue;
			}
		}

		checkTimeouts();
		willClosePending();
	}
}

/*
 * Scan all active handlers (Connections, Listeners)
 * and checks if any of them has run out of time.
 */
void EventLoop::checkTimeouts()
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::vector<IOPollable *> to_timeout;
	to_timeout.reserve(handlers.size());

	for (std::map<int, IOPollable *>::iterator it = handlers.begin(); it != handlers.end(); ++it)
	{
		IOPollable *h = it->second;
		if (h->timeBudgetMs(now) == 0)
			to_timeout.push_back(h);
	}

	for (std::size_t i = 0; i < to_timeout.size(); ++i)
		to_timeout[i]->onTimeout();
}

void EventLoop::stop(void)
{
	Logger::info("EventLoop → STOPPED (begin)");

	std::vector<int> all_fds;
	all_fds.reserve(handlers.size());
	for (std::map<int, IOPollable *>::const_iterator it = handlers.begin(); it != handlers.end(); ++it)
		all_fds.push_back(it->first);

	for (size_t i = 0; i < all_fds.size(); ++i)
		pending_close_fds.insert(all_fds[i]);

	pfds.clear();

	for (std::unordered_set<int>::const_iterator it = pending_close_fds.begin(); it != pending_close_fds.end(); ++it)
	{
		::close(*it);
	}

	handlers.clear();
	pending_close_fds.clear();
	pendingClose.clear();

	Logger::info("EventLoop → STOPPED (complete)");
}
