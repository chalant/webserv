#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "IEventManager.hpp"
#include "IPollfdManager.hpp"
#include "IBufferManager.hpp"
#include "ISocket.hpp"
#include "IServer.hpp"
#include "IRequestHandler.hpp"
#include "ILogger.hpp"
#include "WebservExceptions.hpp"

class EventManager : public IEventManager
{
private:
    IPollfdManager *_pollfdManager;
    IBufferManager *_bufferManager;
    ISocket *_socket;
    IServer *_server;
    IRequestHandler *_requestHandler;
    ILogger *_errorLogger;

    void _handleServerSocketsEvents();
    void _handleServerSocketEvents(ssize_t serverSocketIndex);
    void _handleClientSocketsEvents();
    void _handleClientSocketEvents(ssize_t &clientSocketIndex);
    void _handleFileDescriptorsEvents();
    void _processBufferedResponse(ssize_t clientSocketIndex);
    void _handleException(ssize_t &clientSocketIndex);

public:
    EventManager(IPollfdManager *pollfdManager, IBufferManager *bufferManager, ISocket *socket, IServer *server, IRequestHandler *requestHandler, ILogger *errorLogger);
    ~EventManager();

    virtual void handleEvents();
};

#endif // EVENTMANAGER_HPP
// Path: includes/EventManager.hpp
