#include "../../includes/network/Socket.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>

/*
 * Socket - Class for socket operations.
 *
 * This class provides methods for socket-related operations such as
 * creating a socket, binding to an address, listening for connections,
 * accepting connections, sending and receiving data,
 * controlling socket file descriptor flags, and closing the socket.
 */

// Destructor
Socket::~Socket() {}

// Creates a IPv4 TCP socket
int Socket::socket() const
{
    // return the file descriptor for the socket
    // -1 is returned on error
    // domain: communication domain (AF_INET for IPv4)
    // type: communication semantics (SOCK_STREAM for TCP)
    // protocol: protocol to be used with the socket (0 for default protocol)
    return ::socket(AF_INET, SOCK_STREAM, 0);
}

// Binds the socket to an address
int Socket::bind(int socketDescriptor, int ip, int port) const
{
    // return the result of binding the socket to the address
    // 0 is returned on success, -1 on error
    // socketDescriptor: file descriptor of the socket
    // ip: IP address to bind to
    // port: port number to bind to

    if (port < 0 || port > 65535)
    {
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (ip == 0)
    {
        addr.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        addr.sin_addr.s_addr = htonl(ip);
    }
    return ::bind(socketDescriptor, (struct sockaddr *)&addr, sizeof(addr));
}

// Listens for incoming connections
int Socket::listen(int socketDescriptor, int maxConnections) const
{
    // listen for incoming connections on the socket
    // maxConnections: maximum number of pending connections in the socket's listen queue
    return ::listen(socketDescriptor, maxConnections);
}

// Performs file control operations on a socket
int Socket::fcntl(int socketDescriptor, int cmd, int arg) const
{
    // perform file control operations on the socket
    // cmd: operation to be performed
    // arg: argument to the operation
    return ::fcntl(socketDescriptor, cmd, arg);
}

// Sets the socket to non-blocking mode
int Socket::setNonBlocking(int socketDescriptor) const
{
    // Add non-blocking mode to the current flags of the socket
    // socketDescriptor: file descriptor of the socket
    // Returns: 0 on success, -1 on error
    int flags = ::fcntl(socketDescriptor, F_GETFL, 0);
    if (flags == -1)
    {
        return -1;
    }
    return ::fcntl(socketDescriptor, F_SETFL, flags | O_NONBLOCK);
}

// Accept an incoming connection
std::pair<int, std::pair<std::string, std::string>> Socket::accept(int serverSocketFd) const
{
    // serverSocketFd: file descriptor of the listening socket
    // Returns: file descriptor for the new client socket (int) (-1 on error)
    //          client's IP address (std::string)
    //          client's port number (std::string)

    // Address structure for the client
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Accept the incoming connection and get the client socket descriptor
    int clientSocketFd = ::accept(serverSocketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);

    // Check for errors
    if (clientSocketFd == -1)
    {
        return std::make_pair(-1, std::make_pair("", ""));
    }

    // Convert the binary IP address to a string
    std::string clientIp = inet_ntoa(clientAddr.sin_addr);

    // Retrieve the port number and convert it to host byte order
    uint16_t clientPort_uint16 = ntohs(clientAddr.sin_port);

    // Convert the port number to a string
    std::ostringstream ss;
    ss << clientPort_uint16;
    std::string clientPort = ss.str();

    return std::make_pair(clientSocketFd, std::make_pair(clientIp, clientPort));
}

// Sends data over the socket Non-Blockingly
int Socket::send(int socketDescriptor, const std::vector<char> &data) const
{
    // Send data over the socket
    // socketDescriptor: File descriptor of the socket
    // data: Data to be sent
    // Returns the number of bytes sent
    // -1 is returned on error
    return ::send(socketDescriptor, data.data(), data.size(), MSG_DONTWAIT | MSG_NOSIGNAL);
}

// Sends data over the socket blocking until all data is sent
int Socket::sendAll(int socketDescriptor, const std::vector<char> &data) const
{
    // Send data over the socket blocking until all data is sent
    // socketDescriptor: File descriptor of the socket
    // data: Data to be sent
    // Returns the number of bytes sent
    // -1 is returned on error
    return ::send(socketDescriptor, data.data(), data.size(), MSG_NOSIGNAL);
}

// Receives data from the socket Non-Blockingly
ssize_t Socket::recv(int socketDescriptor, char *buffer, size_t len) const
{

    // receive data from the socket
    // socketDescriptor: file descriptor of the socket
    // returns the received data
    // an empty vector is returned on error
    return ::recv(socketDescriptor, buffer, len, MSG_DONTWAIT);
}