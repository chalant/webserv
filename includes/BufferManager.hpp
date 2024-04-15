#include "IBufferManager.hpp"
#include "IBuffer.hpp"
#include <vector>

class FileBuffer;
class SocketBuffer;

class BufferManager : public IBufferManager
{
private:
    std::vector<IBuffer *> _buffers;

public:
    BufferManager();
    ~BufferManager();

    // Factory methods to create new buffers
    void newFileBuffer(int fileDescriptor);
    void newSocketBuffer(int socketDescriptor);

    // Other methods for managing buffers
    void pushBuffer(IBuffer *buffer, const std::vector<char> &data);
    void flushBuffers();
};
