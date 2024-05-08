#ifndef MOCKSOCKET_HPP
#define MOCKSOCKET_HPP

/*
 * MockSocket class is a mock implementation of the ISocket interface
 * This mock implementation is used for testing purposes to simulate the behavior of the ISocket interface
 * during unit testing without relying on the actual implementation.
 */

#include "../../includes/network/ISocket.hpp" // Include the base interface header
#include <map>                        // Include for std::map

class MockSocket : public ISocket
{
private:
    mutable std::map<int, std::vector<char> > _socketData; // Map to store data for each socket

public:
    // Constructor
    MockSocket();

    // Destructor
    virtual ~MockSocket();

    // Methods implementing the ISocket interface
    // These methods are overridden to provide custom behavior for testing.

    // Creates a socket
    virtual int socket() const;

    // Binds the socket to an IP address and port
    virtual int bind(int fd, int ip, int port) const;

    // Listens for incoming connections
    virtual int listen(int fd, int maxConnections) const;

    // Performs file control operations on a socket
    virtual int fcntl(int fd, int cmd, int arg) const;

    // Sets the socket to non-blocking mode
    virtual int setNonBlocking(int fd) const;

    // Accepts an incoming connection
    virtual std::pair<int, std::pair<std::string, std::string> > accept(int fd) const;

    // Sends data over the socket
    virtual int send(int recipientSocketFd, const std::vector<char> &data) const;

    // Sends data over the socket blocking until all data is sent
    virtual int sendAll(int recipientSocketFd, const std::vector<char> &data) const;

    // Receives data from the socket
    virtual ssize_t recv(int socketDescriptor, char *buffer, size_t len) const;
};

#endif // MOCKSOCKET_HPP
// Path: tests/mock_includes/MockSocket.cpp
