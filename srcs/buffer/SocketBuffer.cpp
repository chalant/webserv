#include "../../includes/buffer/SocketBuffer.hpp"

/*
 * SocketBuffer.hpp
 *
 * Holds buffers intended for socket descriptors.
 */

// Constructor
SocketBuffer::SocketBuffer(ISocket &socket)
    : _size(0),
      _socket(socket)

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
ssize_t SocketBuffer::flush(int socketDescriptor, bool all)
{
    // Attempt to send the buffer to the socket
    ssize_t bytesSent;
    if (all == true) // will block until all data is sent
        bytesSent = this->_socket.sendAll(socketDescriptor, this->_buffer);
    else
        bytesSent = this->_socket.send(socketDescriptor, this->_buffer);

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
        if (static_cast<size_t>(bytesSent) != this->_size) // Not all data was sent
            memmove(&this->_buffer[0], &this->_buffer[bytesSent], bytesSent);
        this->_size -= bytesSent;
    }
    return this->_size; // Return the remaining size of the buffer
}

// Peek at the buffer
std::vector<char> SocketBuffer::peek() const
{
    return this->_buffer; // Return a copy of the buffer
}

// Path: srcs/SocketBuffer.cpp
