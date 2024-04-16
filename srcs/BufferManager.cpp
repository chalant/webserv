#include "../includes/BufferManager.hpp"
#include "../includes/FileBuffer.hpp"
#include "../includes/SocketBuffer.hpp"

BufferManager::BufferManager() {}

BufferManager::~BufferManager()
{
    std::map<int, IBuffer *>::iterator it;
    for (it = this->_buffers.begin(); it != this->_buffers.end(); it++)
    {
        while (it->second->flush(it->first) > 0)
            ; // flush remaining data blockingly
        this->destroyBuffer(it->first);
    }
}

void BufferManager::pushFileBuffer(int fileDescriptor, const std::vector<char> &data)
{
    if (this->_buffers.find(fileDescriptor) == this->_buffers.end())
    {
        this->_buffers[fileDescriptor] = new FileBuffer();
    }
    this->_buffers[fileDescriptor]->push(data);
}

void BufferManager::pushSocketBuffer(int socketDescriptor, const std::vector<char> &data)
{
    if (this->_buffers.find(socketDescriptor) == this->_buffers.end())
    {
        this->_buffers[socketDescriptor] = new SocketBuffer();
    }
    this->_buffers[socketDescriptor]->push(data);
}

ssize_t BufferManager::flushBuffer(int descriptor)
{
    // returns bytes remaining in buffer
    // returns -1 in case of error
    if (this->_buffers.find(descriptor) != this->_buffers.end())
    {
        return (this->_buffers[descriptor]->flush(descriptor));
    }
    return -1;
}

void BufferManager::flushBuffers()
{
    std::map<int, IBuffer *>::iterator it;
    for (it = this->_buffers.begin(); it != this->_buffers.end(); it++)
    {
        this->flushBuffer(it->first);
    }
}

void BufferManager::destroyBuffer(int descriptor)
{
    if (this->_buffers.find(descriptor) != this->_buffers.end())
    {
        delete this->_buffers[descriptor];
        this->_buffers.erase(descriptor);
    }
}

// Path: srcs/FileBuffer.cpp
