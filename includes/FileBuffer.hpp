#ifndef FILEBUFFER_HPP
#define FILEBUFFER_HPP

#include "IBuffer.hpp"
#include <cstring>

#define DEFAULT_MAX_SIZE 33554432
#define DEFAULT_FLUSH_THRESHOLD 32768

class FileBuffer : public IBuffer
{
private:
    std::vector<char> _buffer;
    const int _fileDescriptor;
    const size_t _maxSize;
    const size_t _flushThreshold;
    size_t _size;

public:
    FileBuffer(int fileDescriptor, size_t maxSize = DEFAULT_MAX_SIZE, size_t flushThreshold = DEFAULT_FLUSH_THRESHOLD);
    ~FileBuffer();
    ssize_t push(const std::vector<char> &data);
    ssize_t flush();
};

#endif // FILEBUFFER_HPP
// Path: includes/IBuffer.hpp
