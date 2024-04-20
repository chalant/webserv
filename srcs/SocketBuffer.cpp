#include "../includes/SocketBuffer.hpp"

/*
 * SocketBuffer.hpp
 *
 * Holds buffers intended for socket descriptors.
 */

// Constructor
SocketBuffer::SocketBuffer()
    : _size(0)
{
    // Reserve initial memory for the buffer
    this->_buffer.reserve(4096);
}

// Destructor
SocketBuffer::~SocketBuffer()
{
    // Clear the buffer
    this->_buffer.clear();
}

// Push data into the buffer
ssize_t SocketBuffer::push(const std::vector<char> &data)
{
    // Append data to the buffer
    this->_buffer.insert(this->_buffer.end(), data.begin(), data.end());
    // Update the size of the buffer
    this->_size += data.size();

    return data.size(); // Return the number of bytes pushed
}

// Send the buffer to the socket descriptor
// Returns its remaining size (or -1 in case of error)
ssize_t SocketBuffer::flush(int socketDescriptor)
{
    // Attempt to send the buffer to the socket
    ssize_t bytesSent = send(socketDescriptor, &this->_buffer[0], this->_size, MSG_DONTWAIT | MSG_NOSIGNAL);

    if (bytesSent == -1)
    {
        // Error occurred during send
        // Since we call this only when poll() returns POLLOUT, we assume the error is not related to blocking
        // Clear the buffer and return -1
        this->_buffer.clear();
        return -1;
    }
    else
    {
        // Update buffer state after successful send
        memmove(&this->_buffer[0], &this->_buffer[bytesSent], this->_size - bytesSent);
        this->_size -= bytesSent;
    }

    return this->_size; // Return the remaining size of the buffer
}

// Path: srcs/SocketBuffer.cpp
