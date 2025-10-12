/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiStdinPollable.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/07 13:24:28 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/09 17:02:36 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/CgiProcess.hpp"

CgiProcess::CgiStdinPollable::CgiStdinPollable(CgiProcess &newOwnerProcess) : ownerProcess(newOwnerProcess)
{
}

int CgiProcess::CgiStdinPollable::fd() const
{
	return ownerProcess.stdin_fd;
}

short CgiProcess::CgiStdinPollable::interest() const
{
	return (ownerProcess.stdin_fd >= 0 && !ownerProcess.in_buf.empty()) ? POLLOUT : 0;
}

void CgiProcess::CgiStdinPollable::onReadable()
{
}

void CgiProcess::CgiStdinPollable::onWritable()
{
	ownerProcess.writeToChild();
}

void CgiProcess::CgiStdinPollable::onHangupOrError(short revents)
{
	ownerProcess.stdinHangup(revents);
}
