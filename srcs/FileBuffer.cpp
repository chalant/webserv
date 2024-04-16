#include "../includes/FileBuffer.hpp"

FileBuffer::FileBuffer(int fileDescriptor, size_t maxSize, size_t flushThreshold)
    : _fileDescriptor(fileDescriptor),
      _maxSize(maxSize),
      _flushThreshold(flushThreshold),
      _size(0)
{
    _buffer.reserve(flushThreshold);
}

FileBuffer::~FileBuffer()
{
    this->flush();
    this->_buffer.clear();
}

ssize_t FileBuffer::push(const std::vector<char> &data)
{
    if (this->_size + data.size() > this->_maxSize)
    {
        return -1; // Buffer full
    }

    this->_buffer.insert(this->_buffer.end(), data.begin(), data.end());
    this->_size += data.size();

    return data.size();
}

ssize_t FileBuffer::flush()
{
    if (this->_size < this->_flushThreshold)
    {
        return 0;
    }
    ssize_t bytesWritten = ::write(this->_fileDescriptor, this->_buffer.data(), this->_size);

    if (bytesWritten == -1)
    {
        return -1;
    }
    else
    {
        // Update buffer state after successful write
        this->_size -= bytesWritten;
        if (this->_size > 0)
        {
            memmove(&this->_buffer[0], &this->_buffer[bytesWritten], this->_size);
        }
    }
    return bytesWritten;
}