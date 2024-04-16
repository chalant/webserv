#include "../includes/BufferManager.hpp"
#include "../includes/FileBuffer.hpp"
#include "../includes/SocketBuffer.hpp"

BufferManager::BufferManager()
{
}

BufferManager::~BufferManager()
{
    for (int i = 0; i < this->_buffers.size(); i++)
    {
        delete this->_buffers[i];
    }
    this->_buffers.clear();
}

void BufferManager::newFileBuffer(int fileDescriptor)
{
    this->_buffers.push_back(new FileBuffer(fileDescriptor));
}

void BufferManager::newSocketBuffer(int socketDescriptor)
{
    this->_buffers.push_back(new SocketBuffer(socketDescriptor));
}

void BufferManager::pushBuffer(IBuffer *buffer, const std::vector<char> &data)
{
    buffer->push(data);
}

void BufferManager::flushBuffers()
{
    ssize_t returnValue;
    for (int i = 0; i < this->_buffers.size();)
    {
        returnValue = this->_buffers[i]->flush();
        if (returnValue == -1)
        {
            // Log error
            // Handle error logging here
        }
        else if (dynamic_cast<SocketBuffer *>(this->_buffers[i]) && returnValue == 0)
        {
            delete this->_buffers[i];
            this->_buffers.erase(this->_buffers.begin() + i);
            continue;
        }
        // Increment i only if no elements were erased
        ++i;
    }
}

// Path: srcs/FileBuffer.cpp
