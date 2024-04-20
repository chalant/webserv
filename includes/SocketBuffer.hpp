#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP

/*
 * SocketBuffer.hpp
 *
 * Holds buffers intended for socket descriptors.
 */

#include "IBuffer.hpp"
#include <sys/socket.h>
#include <cstring>
#include <vector>

class SocketBuffer : public IBuffer
{
private:
    std::vector<char> _buffer; // Buffer to hold data
    size_t _size;              // Current size of the buffer

public:
    // Constructor
    SocketBuffer();

    // Destructor
    ~SocketBuffer();

    // Push data into the buffer
    ssize_t push(const std::vector<char> &data);

    // Send the buffer to a socket descriptor
    ssize_t flush(int socketDescriptor);
};

#endif // SOCKETBUFFER_HPP

// Path: includes/Configuration.hpp
