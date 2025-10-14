/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ShutdownPollable.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/13 08:32:35 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 23:26:49 by quentinbeuk   ########   odam.nl         */
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

static std::atomic<ShutdownPollable *> g_active{nullptr};

// INIT
// ____________________________________________________________________________
ShutdownPollable::~ShutdownPollable()
{
	unregisterActive(this);
	uninstallSignals();
	if (read_fd >= 0)
		::close(read_fd);
	if (write_fd >= 0)
		::close(write_fd);
}

bool ShutdownPollable::initialize()
{
	openPipe();
	installSignals();
	registerActive(this);

	return (read_fd >= 0 && write_fd >= 0);
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

void ShutdownPollable::installSignals()
{
	sigset_t set;
	sigemptyset(&set);
	struct sigaction sa;
	sa.sa_handler = &ShutdownPollable::onSignal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	::sigaction(SIGINT, &sa, nullptr);
	::sigaction(SIGTERM, &sa, nullptr);
}

void ShutdownPollable::uninstallSignals()
{
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	::sigaction(SIGINT, &sa, nullptr);
	::sigaction(SIGTERM, &sa, nullptr);
}

// CONTROL
// ____________________________________________________________________________
void ShutdownPollable::registerActive(ShutdownPollable *p) noexcept
{
	g_active.store(p, std::memory_order_relaxed);
}

void ShutdownPollable::unregisterActive(ShutdownPollable *p) noexcept
{
	g_active.compare_exchange_strong(p, nullptr, std::memory_order_relaxed);
}

void ShutdownPollable::onSignal(int) noexcept
{
	ShutdownPollable *shutdown = g_active.load(std::memory_order_relaxed);
	if (!shutdown)
		return;

	// Wake poll()
	ssize_t n = ::write(shutdown->write_fd, "x", 1);
	// n > 0 	→ Wake poll()
	// n == 0	→ Nothing to do
	// n < 0 	→ Nothing to do
	(void)n;
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
	for (;;)
	{
		char buf[100];
		ssize_t n = ::read(read_fd, buf, sizeof(buf));

		if (n <= 0)
			break;
	}
	shutdown_requested.store(true, std::memory_order_relaxed);

	// Drain
	bool expected = false;
	if (draining.compare_exchange_strong(expected, true, std::memory_order_acq_rel))
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
