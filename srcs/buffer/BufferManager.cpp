#include "../../includes/buffer/BufferManager.hpp"
#include "../../includes/buffer/FileBuffer.hpp"
#include "../../includes/buffer/SocketBuffer.hpp"

/*
 * BufferManager Class
 *
 * The BufferManager serves as a centralized mechanism for collecting write and send buffers generated throughout the system.
 *
 * Purpose:
 *   To not block the main thread when sending responses or writing to files.
 *
 * How it Works:
 * - Logging Messages: Various components within the system generate log messages during their operation and register them with the BufferManager.
 * - Sending Responses: The RequestHandler generates responses to client requests and registers them with the BufferManager.
 * - Deferred Writing: Log messages and client responses remain in the buffer until it's time to write them to the output medium, ensuring non-blocking behavior.
 * - Batch Processing: Log messages are processed in batches rather than individually to improve efficiency.
 */

// Constructor
BufferManager::BufferManager(ISocket &socket)
    : _flushThreshold(DEFAULT_FLUSH_THRESHOLD),
      _socket(socket) {}

// Destructor
BufferManager::~BufferManager()
{

    // Clean up all buffers
    std::map<int, IBuffer *>::iterator it = this->_buffers.begin();
    while (it != this->_buffers.end())
    {
        // Flush remaining data in each buffer and then destroy it
        while (it->second->flush(it->first, true) > 0) // flush remaining data blockingly (true)
            ;
        
        // Duplicate itertor to avoid invalidating it
        std::map<int, IBuffer *>::iterator it2 = it;

        // Increment iterator before destroying buffer
        it++;

        // Destroy buffer
        this->destroyBuffer(it2->first);
    }
}

// Push a file buffer into the manager
ssize_t BufferManager::pushFileBuffer(int fileDescriptor, const std::vector<char> &data)
{
    // If the buffer for this file descriptor doesn't exist, create it
    if (this->_buffers.find(fileDescriptor) == this->_buffers.end())
    {
        this->_buffers[fileDescriptor] = new FileBuffer(this->_flushThreshold);
    }
    // Push data into the file buffer
    return this->_buffers[fileDescriptor]->push(data); // returns 1 if a flush is requested
}

// Push a socket buffer into the manager
ssize_t BufferManager::pushSocketBuffer(int socketDescriptor, const std::vector<char> &data)
{
    // If the buffer for this socket descriptor doesn't exist, create it
    if (this->_buffers.find(socketDescriptor) == this->_buffers.end())
    {
        this->_buffers[socketDescriptor] = new SocketBuffer(this->_socket);
    }
    // Push data into the socket buffer
    return this->_buffers[socketDescriptor]->push(data); // returns the number of bytes pushed
}

// Flush the buffer for a specific descriptor
ssize_t BufferManager::flushBuffer(int descriptor)
{
    // Returns bytes remaining in buffer, or -1 in case of error
    if (this->_buffers.find(descriptor) != this->_buffers.end())
    {
        return (this->_buffers[descriptor]->flush(descriptor));
    }
    return -1;
}

// Flush all buffers
void BufferManager::flushBuffers()
{
    std::map<int, IBuffer *>::iterator it;
    for (it = this->_buffers.begin(); it != this->_buffers.end(); it++)
    {
        // Flush each buffer
        this->flushBuffer(it->first);
    }
}

// Destroy the buffer for a specific descriptor
void BufferManager::destroyBuffer(int descriptor)
{
    if (this->_buffers.find(descriptor) != this->_buffers.end())
    {
        // Delete the buffer and remove it from the map
        delete this->_buffers[descriptor];
        this->_buffers.erase(descriptor);
    }
}

// Peek at the buffer for a specific descriptor
std::vector<char> BufferManager::peekBuffer(int descriptor) const
{
    if (this->_buffers.find(descriptor) != this->_buffers.end())
    {
        return this->_buffers.at(descriptor)->peek();
    }
    return std::vector<char>();
}

// Set the flush threshold
void BufferManager::setFlushThreshold(size_t threshold)
{
    this->_flushThreshold = threshold;
}

// Path: srcs/FileBuffer.cpp
