#ifndef IBUFFERMANAGER_HPP
#define IBUFFERMANAGER_HPP

#include "IBuffer.hpp"
#include <vector>

class IBufferManager
{
public:
    virtual ~IBufferManager() = default;

    // methods for managing buffers
    virtual ssize_t pushFileBuffer(int, const std::vector<char> &) = 0;
    virtual ssize_t pushSocketBuffer(int, const std::vector<char> &) = 0;
    virtual ssize_t flushBuffer(int) = 0;
    virtual void flushBuffers() = 0;
    virtual void destroyBuffer(int) = 0;
    virtual std::vector<char> peekBuffer(int) const = 0;
    virtual void setFlushThreshold(size_t) = 0;
};

#endif // IBUFFERMANAGER_HPP
// Path: includes/IBuffer.hpp
