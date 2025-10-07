/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiStdoutPollable.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/07 13:24:30 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/07 13:44:04 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/CgiProcess.hpp"

CgiProcess::CgiStdoutPollable::CgiStdoutPollable(CgiProcess &newOwnerProcess) : ownerProcess(newOwnerProcess)
{
}

int CgiProcess::CgiStdoutPollable::fd() const
{
	return ownerProcess.stdout_fd;
}

short CgiProcess::CgiStdoutPollable::interest() const
{
	return ownerProcess.stdout_fd >= 0 ? POLLIN : 0;
}

void CgiProcess::CgiStdoutPollable::onReadable()
{
	ownerProcess.readFromChild();
}

void CgiProcess::CgiStdoutPollable::onWritable()
{
}

void CgiProcess::CgiStdoutPollable::onHangupOrError(short revents)
{
	ownerProcess.stdoutHangup(revents);
}
