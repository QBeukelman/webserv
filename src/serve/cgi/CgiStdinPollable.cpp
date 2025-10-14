/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiStdinPollable.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/07 13:24:28 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 23:40:02 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/CgiPollable.hpp"

CgiPollable::CgiStdinPollable::CgiStdinPollable(CgiPollable &newOwnerProcess) : ownerProcess(newOwnerProcess)
{
}

int CgiPollable::CgiStdinPollable::fd() const
{
	return ownerProcess.stdin_fd;
}

short CgiPollable::CgiStdinPollable::interest() const
{
	return (ownerProcess.stdin_fd >= 0 && !ownerProcess.in_buf.empty()) ? POLLOUT : 0;
}

void CgiPollable::CgiStdinPollable::onReadable()
{
}

void CgiPollable::CgiStdinPollable::onWritable()
{
	ownerProcess.writeToChild();
}

void CgiPollable::CgiStdinPollable::onHangupOrError(short revents)
{
	ownerProcess.stdinHangup(revents);
}
