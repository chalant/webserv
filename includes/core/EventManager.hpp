#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "IEventManager.hpp"
#include "../pollfd/IPollfdManager.hpp"
#include "../buffer/IBufferManager.hpp"
#include "../network/ISocket.hpp"
#include "../network/IServer.hpp"
#include "../connection/IRequestHandler.hpp"
#include "../logger/ILogger.hpp"
#include "../exception/WebservExceptions.hpp"

typedef std::pair<int, std::pair<int, int> > Triplet_t;

class EventManager : public IEventManager
{
private:
    IPollfdManager &_pollfdManager;
    IBufferManager &_bufferManager;
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
    void _flushBuffer(ssize_t &pollfdIndex, short options = 0);
    void _cleanUp(ssize_t &pollfdIndex, int descriptor, short options = 0);

public:
    EventManager(IPollfdManager &pollfdManager, IBufferManager &bufferManager, IServer &server, IRequestHandler &requestHandler, ILogger &logger);
    ~EventManager();

    virtual void handleEvents();
};

#endif // EVENTMANAGER_HPP
// Path: includes/core/EventManager.hpp
