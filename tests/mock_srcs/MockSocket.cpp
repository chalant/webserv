#include "../mock_includes/MockSocket.hpp"

/*
 * MockSocket class is a mock implementation of the ISocket interface
 * This mock implementation is used for testing purposes to simulate the
 * behavior of the ISocket interface during unit testing without relying on the
 * actual implementation.
 */

// Constructor
MockSocket::MockSocket() {}

// Destructor
MockSocket::~MockSocket() {}

// Creates a socket
int MockSocket::socket() const
{
    // Mock implementation for creating a socket
    // Returns a mock file descriptor for the socket
    return 1;
}

// Binds the socket to an IP address and port
int MockSocket::bind(int fd, int ip, int port) const
{
    // Mock implementation for binding the socket to an address
    // Returns 0 on success
    static_cast<void>(fd);
    static_cast<void>(ip);
    static_cast<void>(port);
    return 0;
}

// Listens for incoming connections
int MockSocket::listen(int fd, int max_connections) const
{
    // Mock implementation for listening for incoming connections
    // Returns 0 on success
    static_cast<void>(fd);
    static_cast<void>(max_connections);
    return 0;
}

// Performs file control operations on a socket
int MockSocket::fcntl(int fd, int cmd, int arg) const
{
    // Mock implementation for performing file control operations on a socket
    // Returns 0 on success
    static_cast<void>(fd);
    static_cast<void>(cmd);
    static_cast<void>(arg);
    return 0;
}

// Sets the socket to non-blocking mode
int MockSocket::setNonBlocking(int fd) const
{
    // Mock implementation for setting the socket to non-blocking mode
    // Returns 0 on success
    static_cast<void>(fd);
    return 0;
}

// Accepts an incoming connection
std::pair<int, std::pair<std::string, std::string> >
MockSocket::accept(int fd) const
{
    // Mock implementation for accepting an incoming connection
    // Returns a mock file descriptor for the new client socket
    // Returns the client's IP address
    // Returns the client's port number
    static_cast<void>(fd);
    return std::make_pair(2, std::make_pair("127.0.0.1", "8080"));
}

// Sends data over the socket
int MockSocket::send(int socket_descriptor, const std::vector<char> &data) const
{
    // Mock implementation for sending data over the socket
    // Returns the number of bytes sent
    m_socket_data[ socket_descriptor ] = data;
    return data.size();
}

// Sends data over the socket blocking until all data is sent
int MockSocket::sendAll(int socket_descriptor,
                        const std::vector<char> &data) const
{
    // Mock implementation for sending data over the socket blocking until all
    // data is sent Returns the number of bytes sent
    m_socket_data[ socket_descriptor ] = data;
    return data.size();
}

// Receives data from the socket
ssize_t MockSocket::recv(int socket_descriptor, char *buffer, size_t len) const
{
    // Mock implementation for receiving data from the socket
    // Returns the number of bytes received
    if (m_socket_data.find(socket_descriptor) != m_socket_data.end())
    {
        std::vector<char> &data =
            m_socket_data[ socket_descriptor ]; // Get reference to data
        size_t i;
        for (i = 0; i < len && i < data.size(); i++)
        {
            buffer[ i ] = data[ i ];
        }

        // Erase characters from data
        data.erase(data.begin(), data.begin() + i);

        // If there are no characters left in data, erase the entry from
        // m_socket_data
        if (data.empty())
        {
            m_socket_data.erase(socket_descriptor);
        }

        return i;
    }
    else
    {
        return -1;
    }
}

// Forcibly bind a socket to a port in use
int MockSocket::setReuseAddr(int fd) const
{
    static_cast<void>(fd);
    return 0;
}

// Path: tests/mock_srcs/MockServer.cpp
