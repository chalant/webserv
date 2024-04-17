#ifndef SOCKETBUFFER_HPP
#define SOCKETBUFFER_HPP

#include "IBuffer.hpp"
#include <sys/socket.h>
#include <cstring>


class SocketBuffer : public IBuffer
{
private:
    std::vector<char> _buffer;
    size_t _size;

public:
    SocketBuffer();
    ~SocketBuffer();
    ssize_t push(const std::vector<char> &data);
    ssize_t flush(int socketDescriptor);
};

#endif // SOCKETBUFFER_HPP
// Path: includes/Configuration.hpp
