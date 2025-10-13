/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ShutdownPollable.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/13 08:32:35 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 09:22:46 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/ShutdownPollable.hpp"

// STATIC
// ____________________________________________________________________________
static void setNonBlocking_(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags >= 0)
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	fcntl(fd, F_SETFD, FD_CLOEXEC);
}

// INIT
// ____________________________________________________________________________
ShutdownPollable &ShutdownPollable::instance()
{
	static ShutdownPollable singleton;
	return (singleton);
}

bool ShutdownPollable::initialize()
{
	if (read_fd != -1)
		return (true);

	openPipe();

	if (read_fd == -1 || write_fd == -1)
		return (false);

	installSignals();

	return (true);
}

void ShutdownPollable::installSignals()
{
	std::signal(SIGINT, &ShutdownPollable::onSignal);
	std::signal(SIGTERM, &ShutdownPollable::onSignal);
}

void ShutdownPollable::openPipe()
{
	int p[2];
	if (::pipe(p) != 0)
		return;

	read_fd = p[0];
	write_fd = p[1];
	setNonBlocking_(read_fd);
	setNonBlocking_(write_fd);
}

ShutdownPollable::~ShutdownPollable()
{
	if (read_fd != -1)
		::close(read_fd);
	if (write_fd != -1)
		::close(write_fd);
}

// CONTROL
// ____________________________________________________________________________
void ShutdownPollable::onSignal(int)
{
	auto &s = ShutdownPollable::instance();
	s.shutdown_requested.store(true, std::memory_order_relaxed);

	(void)!::write(s.write_fd, "x", 1);
}

void ShutdownPollable::requestShutdown()
{
	Logger::info("ShutdownPollable::requestShutdown()");
	onSignal(0);
}

void ShutdownPollable::setOnBeginDrain(std::function<void()> cb)
{
	on_begin_drain = std::move(cb);
}

// I/O
// ____________________________________________________________________________
int ShutdownPollable::fd() const
{
	return (read_fd);
}

short ShutdownPollable::interest() const
{
	return (POLLIN);
}

void ShutdownPollable::onReadable()
{
	char buf[100];
	while (::read(read_fd, buf, sizeof(buf)) > 0)
	{
	}

	if (!draining.exchange(true, std::memory_order_relaxed))
	{
		if (on_begin_drain)
			on_begin_drain();
	}
}

void ShutdownPollable::onWritable()
{
}

void ShutdownPollable::onHangupOrError(short)
{
}
