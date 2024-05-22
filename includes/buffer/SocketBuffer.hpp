#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP

/*
 * SocketBuffer.hpp
 *
 * Holds buffers intended for socket descriptors.
 */

#include "../network/ISocket.hpp"
#include "IBuffer.hpp"
#include <cstring>
#include <vector>

class SocketBuffer : public IBuffer
{
private:
    std::vector<char> _buffer; // Buffer to hold data
    ISocket &_socket;          // Socket object for sending data

public:
    // Constructor
    SocketBuffer(ISocket &socket);

    // Destructor
    ~SocketBuffer();

    // Push data into the buffer
    ssize_t push(const std::vector<char> &data);

    // Send the buffer to a socket descriptor
    ssize_t flush(int socketDescriptor, bool all = false);

    // Peek at the buffer
    std::vector<char> peek() const;
};

#endif // SOCKETBUFFER_HPP

// Path: includes/buffer/SocketBuffer.hpp
