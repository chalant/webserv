#include "../../includes/buffer/SocketBuffer.hpp"

/*
 * SocketBuffer.hpp
 *
 * Holds buffers intended for socket descriptors.
 */

// Constructor
SocketBuffer::SocketBuffer(ISocket &socket) : m_socket(socket)

{
    // Reserve initial memory for the buffer
    this->m_buffer.reserve(4096);
}

// Destructor
SocketBuffer::~SocketBuffer()
{
    // Clear the buffer
    this->m_buffer.clear();
}

// Push data into the buffer
ssize_t SocketBuffer::push(const std::vector<char> &data)
{
    // Append data to the buffer
    this->m_buffer.insert(this->m_buffer.end(), data.begin(), data.end());

    // Return the number of bytes pushed
    return data.size();
}

// Send the buffer to the socket descriptor
// Returns its remaining size (or -1 in case of error)
ssize_t SocketBuffer::flush(int socket_descriptor, bool all)
{
    // Attempt to send the buffer to the socket
    ssize_t bytes_sent = 0;
    if (all == true) // will block until all data is sent
        bytes_sent = this->m_socket.sendAll(socket_descriptor, this->m_buffer);
    else // will send as much data as possible without blocking
        bytes_sent = this->m_socket.send(socket_descriptor, this->m_buffer);

    if (bytes_sent == -1)
    {
        // Error occurred during send
        // Since we call this only when poll() returns POLLOUT, we assume the
        // error is not related to blocking Clear the buffer and return -1
        this->m_buffer.clear();
        return -1;
    }
    else
    {
        // Update buffer state after successful send
        size_t bytesRemaining =
            this->m_buffer.size() - static_cast<size_t>(bytes_sent);
        memmove(&this->m_buffer[ 0 ], &this->m_buffer[ bytes_sent ],
                bytesRemaining);
        this->m_buffer.resize(bytesRemaining);
    }
    return this->m_buffer.size(); // Return the remaining size of the buffer
}

// Peek at the buffer
std::vector<char> SocketBuffer::peek() const
{
    return this->m_buffer; // Return a copy of the buffer
}

// Path: srcs/SocketBuffer.cpp
