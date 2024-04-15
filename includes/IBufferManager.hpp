#ifndef IBUFFERMANAGER_HPP
#define IBUFFERMANAGER_HPP

#include "IBuffer.hpp"
#include <vector>

class IBufferManager
{
public:
    virtual ~IBufferManager() = default;

    // Factory methods to create new buffers
    virtual void newFileBuffer(int) = 0;
    virtual void newSocketBuffer(int) = 0;

    // Other methods for managing buffers
    virtual void pushBuffer(IBuffer *, const std::vector<char> &) = 0;
    virtual void flushBuffers() = 0;
};

#endif // IBUFFERMANAGER_HPP
// Path: includes/IBuffer.hpp
