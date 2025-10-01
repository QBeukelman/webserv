/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IOPollable.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/09 15:13:09 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/01 14:34:21 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef IOPOLLABLE_HPP
#define IOPOLLABLE_HPP

#include <chrono>
#include <climits>
#include <poll.h>

/*
 * The `EventLoop` only knows how to `poll()` the file descriptors,
 * and call back to something. But something could be different kinds of objects:
 *
 * 		- A `Listener` (waiting for new connections on a socket).
 * 		- A `Connection` (handling reads/writes on a client socket).
 *
 * Each of these behaves differently when POLLIN/POLLOUT fires.
 * Instead of teaching the loop about each class,
 * we give them all a common interface: IOPollable.
 */
struct IOPollable
{
	virtual ~IOPollable();

	virtual int fd() const = 0;
	virtual short interest() const = 0;
	virtual void onReadable() = 0;
	virtual void onWritable() = 0;
	virtual void onHangupOrError(short revents) = 0;

	// Timeout
	virtual int timeBudgetMs(std::chrono::steady_clock::time_point now) const;
	virtual void onTimeout();
};

#endif
