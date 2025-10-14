/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiStdoutPollable.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/07 13:24:30 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/13 23:40:02 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/CgiPollable.hpp"

CgiPollable::CgiStdoutPollable::CgiStdoutPollable(CgiPollable &newOwnerProcess) : ownerProcess(newOwnerProcess)
{
}

int CgiPollable::CgiStdoutPollable::fd() const
{
	return ownerProcess.stdout_fd;
}

short CgiPollable::CgiStdoutPollable::interest() const
{
	return ownerProcess.stdout_fd >= 0 ? POLLIN : 0;
}

void CgiPollable::CgiStdoutPollable::onReadable()
{
	ownerProcess.readFromChild();
}

void CgiPollable::CgiStdoutPollable::onWritable()
{
}

void CgiPollable::CgiStdoutPollable::onHangupOrError(short revents)
{
	ownerProcess.stdoutHangup(revents);
}
