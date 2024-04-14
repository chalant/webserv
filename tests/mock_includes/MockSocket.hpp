#ifndef MOCKSOCKET_HPP
#define MOCKSOCKET_HPP

/*
 * MockSocket class is a mock implementation of the ISocket interface
 * This mock implementation is used for testing purposes to simulate the behavior of the ISocket interface
 * during unit testing without relying on the actual implementation.
 */

#include "../../includes/ISocket.hpp" // Include the base interface header
#include <map>                        // Include for std::map

class MockSocket : public ISocket
{
private:
    mutable std::map<int, std::vector<char>> _socketData; // Map to store data for each socket

public:
    // Constructor
    MockSocket();

    // Destructor
    virtual ~MockSocket();

    // Methods implementing the ISocket interface
    // These methods are overridden to provide custom behavior for testing.

    // Creates a socket
    virtual int socket(int domain, int type, int protocol) const;

    // Binds the socket to an IP address and port
    virtual int bind(int fd, const std::string &ip, int port) const;

    // Listens for incoming connections
    virtual int listen(int fd, int maxConnections) const;

    // Performs file control operations on a socket
    virtual int fcntl(int fd, int cmd, int arg) const;

    // Accepts an incoming connection
    virtual std::pair<int, std::pair<std::string, std::string>> accept(int fd) const;

    // Sends data over the socket
    virtual int send(int recipientSocketFd, const std::vector<char> &data) const;

    // Receives data from the socket
    virtual ssize_t recv(int socketDescriptor, char *buffer, size_t len) const;

    // Polls the socket for events
    virtual int poll(struct pollfd *fds, nfds_t nfds) const;
};

#endif // MOCKSOCKET_HPP
// Path: tests/mock_includes/MockSocket.cpp
