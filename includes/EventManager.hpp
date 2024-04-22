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
    IPollfdManager &_pollfdManager;
    IBufferManager &_bufferManager;
    ISocket &_socket;
    IServer &_server;
    IRequestHandler &_requestHandler;
    ILogger &_logger;

    // Event handling functions for different types of files
    void _handleRegularFileEvents(ssize_t &pollfdIndex, short events);
    void _handleServerSocketEvents(ssize_t pollfdIndex, short events);
    void _handleClientSocketEvents(ssize_t &pollfdIndex, short events);
    void _handlePipeEvents(ssize_t &pollfdIndex, short events);

    // helper functions
    void _handleRequest(ssize_t &pollfdIndex);
    void _handleClientException(ssize_t &pollfdIndex, short events);
    void _flushClientBuffer(ssize_t pollfdIndex);
    void _cleanUp(ssize_t &pollfdIndex, int descriptor);

public:
    EventManager(IPollfdManager &pollfdManager, IBufferManager &bufferManager, ISocket &socket, IServer &server, IRequestHandler &requestHandler, ILogger &logger);
    ~EventManager();

    virtual void handleEvents();
};

#endif // EVENTMANAGER_HPP
// Path: includes/EventManager.hpp
