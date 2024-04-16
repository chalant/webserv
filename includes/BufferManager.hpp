#include "IBufferManager.hpp"
#include "IBuffer.hpp"
#include <map>

class FileBuffer;
class SocketBuffer;

class BufferManager : public IBufferManager
{
private:
    std::map<int, IBuffer *> _buffers;

public:
    BufferManager();
    ~BufferManager();

    // Other methods for managing buffers
    void pushFileBuffer(int fileDescriptor, const std::vector<char> &data);
    void pushSocketBuffer(int socketDescriptor, const std::vector<char> &data);
    ssize_t flushBuffer(int descriptor);
    void flushBuffers();
    void destroyBuffer(int descriptor);
};
