#ifndef BUFFERMANAGER_HPP
#define BUFFERMANAGER_HPP

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

#include "IBufferManager.hpp"
#include "IBuffer.hpp"
#include <map>

// Forward declarations
class FileBuffer;
class SocketBuffer;

class BufferManager : public IBufferManager
{
private:
    std::map<int, IBuffer *> _buffers;

public:
    // Constructor
    BufferManager();

    // Destructor
    ~BufferManager();

    // Push a file buffer into the manager
    void pushFileBuffer(int fileDescriptor, const std::vector<char> &data);

    // Push a socket buffer into the manager
    void pushSocketBuffer(int socketDescriptor, const std::vector<char> &data);

    // Flush the buffer for a specific descriptor
    ssize_t flushBuffer(int descriptor);

    // Flush all buffers
    void flushBuffers();

    // Destroy the buffer for a specific descriptor
    void destroyBuffer(int descriptor);
};

#endif // BUFFERMANAGER_HPP

// Path: includes/IBufferManager.hpp
