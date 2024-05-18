#ifndef SOCKET_HPP
#define SOCKET_HPP

/*
 * Socket - Class for socket operations.
 *
 * This class provides methods for socket-related operations such as
 * creating a socket, binding to an address, listening for connections,
 * accepting connections, sending and receiving data, polling for events,
 * controlling socket file descriptor flags, and closing the socket.
 */

#ifdef __APPLE__ // Check if compiling on macOS
	#define MSG_NOSIGNAL SO_NOSIGPIPE
#endif

#include "ISocket.hpp"
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Class for socket operations
class Socket : public ISocket
{
public:
    // Destructor
    virtual ~Socket();

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

#endif // SOCKET_HPP
       // Path: includes/ISocket.hpp