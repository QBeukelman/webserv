/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serve.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: qbeukelm <qbeukelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 19:43:45 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/09/08 14:08:20 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include <arpa/inet.h>
// #include <cstdio>
// #include <poll.h>
// #include <string>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <vector>

// #include "log/Logger.hpp"
// #include "serve/serve.hpp"

// int create_server_socket(int port);
// void accept_new_connection(int server_socket, std::vector<pollfd> &poll_fds);
// void read_data_from_socket(int i, std::vector<pollfd> &poll_fds);

// int create_server_socket(int port);
// void accept_new_connection(int server_socket, std::vector<pollfd> &poll_fds);
// void read_data_from_socket(int i, std::vector<pollfd> &poll_fds);

// void serve(const ServerConfig &config)
// {
// 	int server_socket;
// 	int status;
// 	int port = config.getPort();

// 	// To monitor client sockets:
// 	std::vector<pollfd> poll_fds(1); // vector of socket file descriptors

// 	// Create server listening socket
// 	server_socket = create_server_socket(port);
// 	if (server_socket == -1)
// 	{
// 		Logger::error("Error creating server socket");
// 		return;
// 	}

// 	// Listen to port via socket
// 	Logger::info("[Server] Listening on port " + std::to_string(port));
// 	status = listen(server_socket, 10);
// 	if (status != 0)
// 	{
// 		Logger::error("Listen error " + std::string(strerror(errno)));
// 		return;
// 	}

// 	// Add the listening server socket to array
// 	// with notification when the socket can be read
// 	poll_fds[0].fd = server_socket;
// 	poll_fds[0].events = POLLIN;

// 	while (1)
// 	{ // Main loop
// 		// Poll sockets to see if they are ready (2 second timeout)
// 		status = poll(poll_fds.data(), poll_fds.size(), 2000);
// 		if (status == -1)
// 		{
// 			Logger::error("Poll error " + std::string(strerror(errno)));
// 			return;
// 		}
// 		else if (status == 0)
// 		{
// 			// None of the sockets are ready
// 			Logger::info("Waiting...");
// 			continue;
// 		}

// 		// Loop on our array of sockets
// 		for (unsigned long i = 0; i < poll_fds.size(); i++)
// 		{
// 			if ((poll_fds[i].revents & POLLIN) != 1)
// 			{
// 				// The socket is not ready for reading
// 				// stop here and continue the loop
// 				continue;
// 			}
// 			Logger::info("[" + std::to_string(poll_fds[i].fd) + "Ready for I/O operation");
// 			// The socket is ready for reading!
// 			if (poll_fds[i].fd == server_socket)
// 			{
// 				// Socket is our listening server socket
// 				accept_new_connection(server_socket, poll_fds);
// 			}
// 			else
// 			{
// 				// Socket is a client socket, read it
// 				read_data_from_socket(i, poll_fds);
// 			}
// 		}
// 	}
// }

// int create_server_socket(int port)
// {
// 	struct sockaddr_in sa;
// 	int socket_fd;
// 	int status;

// 	// Prepare the address and port for the server socket
// 	memset(&sa, 0, sizeof sa);
// 	sa.sin_family = AF_INET;					 // IPv4
// 	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
// 	sa.sin_port = htons(port);

// 	// Create listening socket
// 	socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
// 	if (socket_fd == -1)
// 	{
// 		fprintf(stderr, "[Server] Socket error: %s\n", strerror(errno));
// 		return (-1);
// 	}
// 	printf("[Server] Created server socket fd: %d\n", socket_fd);

// 	// Bind socket to address and port
// 	status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
// 	if (status != 0)
// 	{
// 		fprintf(stderr, "[Server] Bind error: %s\n", strerror(errno));
// 		return (-1);
// 	}
// 	printf("[Server] Bound socket to localhost port %d\n", port);

// 	return (socket_fd);
// }

// void accept_new_connection(int server_socket, std::vector<pollfd> &poll_fds)
// {
// 	int client_fd;
// 	int status;

// 	client_fd = accept(server_socket, NULL, NULL);
// 	if (client_fd == -1)
// 	{
// 		fprintf(stderr, "[Server] Accept error: %s\n", strerror(errno));
// 		return;
// 	}
// 	poll_fds.emplace_back();
// 	poll_fds.back().fd = client_fd;
// 	poll_fds.back().events = POLLIN;

// 	printf("[Server] Accepted new connection on client socket %d.\n", client_fd);

// 	std::string msg = "Welcome. You are client fd [" + std::to_string(client_fd) + "]\n";
// 	status = send(client_fd, msg.data(), msg.length(), 0);
// 	if (status == -1)
// 	{
// 		fprintf(stderr, "[Server] Send error to client %d: %s\n", client_fd, strerror(errno));
// 	}
// }

// void read_data_from_socket(int i, std::vector<pollfd> &poll_fds)
// {
// 	char buffer[BUFSIZ];
// 	int bytes_read;
// 	int sender_fd;

// 	sender_fd = poll_fds[i].fd;
// 	memset(&buffer, '\0', sizeof buffer);
// 	bytes_read = recv(sender_fd, buffer, BUFSIZ, 0);
// 	if (bytes_read <= 0)
// 	{
// 		if (bytes_read == 0)
// 		{
// 			printf("[%d] Client socket closed connection.\n", sender_fd);
// 		}
// 		else
// 		{
// 			fprintf(stderr, "[Server] Recv error: %s\n", strerror(errno));
// 		}
// 		close(sender_fd); // Close socket
// 		poll_fds[i] = poll_fds.back();
// 		poll_fds.pop_back();
// 	}
// 	else
// 	{
// 		printf("[%d] Got message: %s", sender_fd, buffer);
// 	}
// }
