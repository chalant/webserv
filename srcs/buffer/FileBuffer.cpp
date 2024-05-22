#include "../../includes/buffer/FileBuffer.hpp"

/*
 * FileBuffer.cpp
 *
 * Holds buffers intended for file descriptors.
 *
 */

// Constructor
FileBuffer::FileBuffer(size_t flushThreshold, size_t maxSize)
    : _flushThreshold(flushThreshold), _maxSize(maxSize)
{
    // Reserve memory for the buffer based on the flush threshold
    _buffer.reserve(flushThreshold);
}

// Destructor
FileBuffer::~FileBuffer()
{
    // Clear the buffer
    this->_buffer.clear();
}

// Push data into the buffer
// Returns -1 if the max size is reached, 1 if the flush threshold is reached,
// or 0 otherwise
ssize_t FileBuffer::push(const std::vector<char> &data)
{
    // Check if the absolute max size of the buffer is reached
    if (this->_buffer.size() + data.size() > this->_maxSize)
    {
        return -1; // Buffer full, cannot push more data
    }

    // Append the data to the buffer
    this->_buffer.insert(this->_buffer.end(), data.begin(), data.end());

    // Return 1 to request a flush if the buffer size exceeds the flush
    // threshold Otherwise, return 0
    return (this->_buffer.size() > this->_flushThreshold);
}

// Flush the buffer to the file descriptor
// Returns the remaining size of the buffer (or -1 in case of error)
ssize_t FileBuffer::flush(int fileDescriptor, bool regardlessOfThreshold)
{
    // Check if the buffer size is less than the flush threshold
    if (regardlessOfThreshold == false &&
        this->_buffer.size() < this->_flushThreshold)
    {
        return this->_buffer
            .size(); // Not enough data to flush, do nothing, just return
                     // the remaining size of the buffer
    }

    // Write the buffer to the file descriptor
    ssize_t bytesWritten =
        ::write(fileDescriptor, this->_buffer.data(), this->_buffer.size());

    if (bytesWritten == -1)
    {
        return -1; // Error writing to file descriptor
    }
    else
    {
        // Update buffer state after successful write
        size_t bytesRemaining = this->_buffer.size() - bytesWritten;
        // Shift the remaining data to the beginning of the buffer
        memmove(&this->_buffer[ 0 ], &this->_buffer[ bytesWritten ],
                bytesRemaining);
        this->_buffer.resize(bytesRemaining);
    }
    return this->_buffer.size(); // Return the remaining size of the buffer
}

// Peek at the buffer
std::vector<char> FileBuffer::peek() const
{
    return this->_buffer; // Return a copy of the buffer
}

// Path: includes/IBuffer.hpp
