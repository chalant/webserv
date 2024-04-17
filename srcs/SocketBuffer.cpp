#include "../includes/SocketBuffer.hpp"

SocketBuffer::SocketBuffer()
    : _size(0)
{
    this->_buffer.reserve(4096);
}

SocketBuffer::~SocketBuffer()
{
    this->_buffer.clear();
}

ssize_t SocketBuffer::push(const std::vector<char> &data)
{

    // Append data to the buffer
    this->_buffer.insert(this->_buffer.end(), data.begin(), data.end());
    // Update the size of the buffer
    this->_size += data.size();

    return data.size();
}

// Sends the buffer to the socket descriptor
// returns its remaining size (or -1 in case of error)
ssize_t SocketBuffer::flush(int socketDescriptor)
{
    ssize_t bytesSent = send(socketDescriptor, &this->_buffer[0], this->_size, MSG_DONTWAIT | MSG_NOSIGNAL);
    if (bytesSent == -1)
    {
        // without checking errno, we do not know the cause
        // but since we call this only when poll() returns POLLOUT
        // we will assume that the error is not related to blocking
        this->_buffer.clear();
        return -1;
    }
    else
    {
        memmove(&this->_buffer[0], &this->_buffer[bytesSent], this->_size - bytesSent);
        this->_size -= bytesSent;
    }
    return this->_size;
}