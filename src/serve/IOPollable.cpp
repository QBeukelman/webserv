/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IOPollable.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: quentinbeukelman <quentinbeukelman@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/10/01 14:25:32 by quentinbeuk   #+#    #+#                 */
/*   Updated: 2025/10/01 14:27:42 by quentinbeuk   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serve/IOPollable.hpp"

IOPollable::~IOPollable()
{
}

int IOPollable::timeBudgetMs(std::chrono::steady_clock::time_point now) const
{
	(void)now;
	return (INT_MAX);
}

void IOPollable::onTimeout()
{
	onHangupOrError(POLLHUP);
}
