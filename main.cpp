#include "includes/Configuration.hpp"
// #include "includes/ConfigurationLoader.hpp"
#include "includes/Server.hpp"
#include "includes/ClientHandler.hpp"
#include "includes/BufferManager.hpp"
#include "includes/BufferManager.hpp"
#include "includes/EventManager.hpp"
#include "includes/Logger.hpp"
#include "includes/ExceptionHandler.hpp"
#include "includes/Socket.hpp"
#include "includes/RequestHandler.hpp"
#include "includes/PollingService.hpp"

/*
 * webserv Workflow:
 *
 * Upon launch, the configuration class reads and parses the configuration file,
 * providing settings for other components.
 * The Server class sets up a TCP connection, monitors events using a poll mechanism,
 * and accepts incoming connections.
 * When an event is detected by the poll mechanism, the RequestHandler class coordinates request processing.
 * It utilizes the ClientHandler to communicate with the client,
 * the RequestParser to parse the request, the Router to select the appropriate AResponseGenerator,
 * and finally, the AResponseGenerator to generate a Response.
 * The generated Response is then sent back to the client by the ClientHandler.
 * After processing a request, the ILogger class writes the log buffers to files.
 * This process continues in a loop.
 */

int main(int argc, char **argv)
{
    ILogger *errorLogger;
    ILogger *accessLogger;
    IExceptionHandler *exceptionHandler;
    ISocket *socket;
    IConfiguration *configuration;
    IPollfdManager *pollfdManager;
    IBufferManager *bufferManager;
    IServer *server;
    IEventManager *eventManager;
    IRequestHandler *requestHandler;
    IClientHandler *clientHandler;
    IPollingService *pollingService;
    // IRequestParser *requestParser;
    // IRouter *router;
    // IResponseGenerator *requestHandler;

    try
    {
        // Instantiate the bufferManager.
        bufferManager = new BufferManager();

        // Instantiate the Socket instance.
        socket = new Socket();

        // Instantiate the errorLogger.
        errorLogger = new Logger(ERRORLOGGER, bufferManager);

        // Instantiate the exceptionHandler.
        exceptionHandler = new ExceptionHandler(errorLogger);

        // Instantiate the IConfiguration instance.
        configuration = new Configuration();

        // parse the configuration file

        // Configure errorLogger
        errorLogger->configure(configuration);

        // Instantiate the accessLogger, primarily for use by the ResponseGenerator class to handle access event logging.
        accessLogger = new Logger(ACCESSLOGGER, bufferManager, configuration);

        // Instantiate the Server. Sets up connectivity, responsible for polling and accepting connections.
        server = new Server(socket, pollfdManager, configuration, errorLogger);

        // Instantiate the PollfdManager. Manages the pollfd array
        pollfdManager = new PollfdManager(configuration, errorLogger, accessLogger, server);

        // Instantiate the PollingService. Manages polling operations.
        pollingService = new PollingService(pollfdManager);

        // Instantiate the RequestHandler. Coordinates request processing utilizing the poll fd array.
        requestHandler = new RequestHandler(socket, pollfdManager, bufferManager, configuration, errorLogger, accessLogger, exceptionHandler);

        // Instantiate the EventManager. Manages events
        eventManager = new EventManager(pollfdManager, bufferManager, socket, server, requestHandler, errorLogger);

        // Start the webserv core cycle.
        while (true)
        {
            try
            {
                // Poll events.
                pollingService->pollEvents();

                // Handle events.
                eventManager->handleEvents();
            }
            catch (WebservException &e)
            {
                exceptionHandler->handleException(e, "webserv core cycle: ");
            }
        }
    }
    catch (WebservException &e)
    {
        exceptionHandler->handleException(e, "webserv setup: ");
    }
}
