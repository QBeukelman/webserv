/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qbeukelm <qbeukelm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 09:13:07 by quentinbeuk       #+#    #+#             */
/*   Updated: 2025/09/08 12:45:34 by qbeukelm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "config/ServerConfig.hpp"

#include <cstring>
#include <sstream>
#include <stdexcept>

// POSIX / sockets
#include <arpa/inet.h>	// inet_pton, inet_ntop
#include <fcntl.h>		// fcntl(), O_NONBLOCK
#include <netinet/in.h> // sockaddr_in, htons(), htonl(), INADDR_ANY
#include <sys/socket.h> // socket(), bind(), listen()
#include <sys/types.h>	// basic system types
#include <unistd.h>		// close()

class Listener
{
  public:
	// Construct and start listening on the given TCP port (IPv4).
	// Binds to 0.0.0.0 (all interfaces).
	explicit Listener(const std::string &ip, unsigned short port);

	~Listener();

	// The listening file descriptor (ready to add to poll()/select()/kqueue/epoll).
	int getFd() const;

  private:
	int fd; // listening socket fd

	void setReuseAddress(); // SO_REUSEADDR
	void setNonBlocking();	// O_NONBLOCK
	void bindAndListen(const std::string &ip, unsigned short port);
};

#endif // LISTENER_HPP
