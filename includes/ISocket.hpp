#ifndef ISOCKET_HPP
#define ISOCKET_HPP

/*
 * ISocket - Interface for socket operations.
 * 
 * This interface defines methods for socket-related operations such as 
 * creating a socket, binding to an address, listening for connections,
 * accepting connections, sending and receiving data, polling for events,
 * controlling socket file descriptor flags, and closing the socket.
 */

#include <sys/socket.h>
#include <poll.h>
#include <vector>
#include <string>

// Interface for socket operations
class ISocket
{
public:
    virtual ~ISocket() {};

    // Creates a socket
    virtual int socket(int domain, int type, int protocol) const = 0;

    // Binds the socket to an IP address and port
    virtual int bind(int fd, const std::string &ip, int port) const = 0;

    // Listens for incoming connections
    virtual int listen(int fd, int maxConnections) const = 0;

    // Performs file control operations on a socket
    virtual int fcntl(int fd, int cmd, int arg) const = 0;

    // Accepts an incoming connection
    virtual std::pair<int, std::pair<std::string, std::string>> accept(int fd) const = 0;

    // Sends data over the socket
    virtual int send(int recipientSocketFd, const std::vector<char> &data) const = 0;

    // Receives data from the socket
    virtual ssize_t recv(int socketDescriptor, char *buffer, size_t len) const = 0;

    // Polls the socket for events
    virtual int poll(struct pollfd *fds, nfds_t nfds) const = 0;
};

#endif // ISOCKET_HPP
// Path: includes/Server.hpp
