#ifndef FILEBUFFER_HPP
#define FILEBUFFER_HPP

/*
 * FileBuffer.hpp
 *
 * Holds buffers intended for file descriptors.
 * The buffer is flushed when the threshold is reached or when the buffer is full.
 */

#include "IBuffer.hpp"
#include <cstring>
#include <vector>

// Default values for maximum size and flush threshold
#define DEFAULT_MAX_SIZE 33554432     // 32 MB
#define DEFAULT_FLUSH_THRESHOLD 32768 // 32 KB

class FileBuffer : public IBuffer
{
private:
    std::vector<char> _buffer;    // Buffer to hold data
    const size_t _maxSize;        // Maximum size of the buffer
    const size_t _flushThreshold; // Threshold at which to flush the buffer
    size_t _size;                 // Current size of the buffer

public:
    // Constructor with optional parameters
    FileBuffer(size_t maxSize = DEFAULT_MAX_SIZE, size_t flushThreshold = DEFAULT_FLUSH_THRESHOLD);

    // Destructor
    ~FileBuffer();

    // Push data into the buffer
    ssize_t push(const std::vector<char> &data);

    // Flush the buffer to a file descriptor
    ssize_t flush(int fileDescriptor);
};

#endif // FILEBUFFER_HPP

// Path: includes/IBuffer.hpp
