#include "../includes/FileBuffer.hpp"

/*
 * FileBuffer.cpp
 *
 * Holds buffers intended for file descriptors.
 * The buffer is flushed when the threshold is reached or when the buffer is full.
 */

// Constructor
FileBuffer::FileBuffer(size_t maxSize, size_t flushThreshold)
    : _maxSize(maxSize),
      _flushThreshold(flushThreshold),
      _size(0)
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
ssize_t FileBuffer::push(const std::vector<char> &data)
{
    if (this->_size + data.size() > this->_maxSize)
    {
        return -1; // Buffer full, cannot push more data
    }

    // Append the data to the buffer
    this->_buffer.insert(this->_buffer.end(), data.begin(), data.end());
    this->_size += data.size();

    return data.size(); // Return the number of bytes pushed
}

// Flush the buffer to the file descriptor
// Returns the remaining size of the buffer (or -1 in case of error)
ssize_t FileBuffer::flush(int fileDescriptor)
{
    // Check if the buffer size is less than the flush threshold
    if (this->_size < this->_flushThreshold)
    {
        return this->_size; // Not enough data to flush, do nothing, just return the remaining size of the buffer
    }

    // Write the buffer to the file descriptor
    ssize_t bytesWritten = ::write(fileDescriptor, this->_buffer.data(), this->_size);

    if (bytesWritten == -1)
    {
        return -1; // Error writing to file descriptor
    }
    else
    {
        // Update buffer state after successful write
        this->_size -= bytesWritten;
        if (this->_size > 0)
        {
            // Shift the remaining data to the beginning of the buffer
            memmove(&this->_buffer[0], &this->_buffer[bytesWritten], this->_size);
        }
    }
    return this->_size; // Return the remaining size of the buffer
}

// Path: includes/IBuffer.hpp
