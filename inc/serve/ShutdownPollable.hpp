/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ShutdownPollable.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/13 08:26:27 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 09:22:57 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHUTDOWNPOLLABLE_HPP
#define SHUTDOWNPOLLABLE_HPP

#include "log/Logger.hpp"
#include "serve/IOPollable.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <fcntl.h>
#include <functional>
#include <poll.h>
#include <sys/types.h>
#include <unistd.h>

class ShutdownPollable : public IOPollable
{
  private:
	int read_fd = -1;
	int write_fd = -1;

	std::atomic<bool> shutdown_requested{false};
	std::atomic<bool> draining{false};

	std::function<void()> on_begin_drain;

	ShutdownPollable() = default;
	~ShutdownPollable();

	static void onSignal(int);
	void openPipe();
	void installSignals();

  public:
	static ShutdownPollable &instance();
	bool initialize();
	void requestShutdown();
	void setOnBeginDrain(std::function<void()> cb);

	// IOPollable
	int fd() const override;
	short interest() const override;
	void onReadable() override;
	void onWritable() override;
	void onHangupOrError(short) override;
};

#endif
