#include "includes/Configuration.hpp"
// #include "includes/ConfigurationLoader.hpp"
#include "includes/Server.hpp"
#include "includes/ClientHandler.hpp"
#include "includes/LoggerConfiguration.hpp"
#include "includes/BufferManager.hpp"
#include "includes/SessionManager.hpp"
#include "includes/EventManager.hpp"
#include "includes/Factory.hpp"
#include "includes/Logger.hpp"
#include "includes/ExceptionHandler.hpp"
#include "includes/Socket.hpp"
#include "includes/RequestHandler.hpp"
#include "includes/PollingService.hpp"
#include "includes/Router.hpp"

/*
 * webserv Workflow:
 *
 * Upon launch, the configuration class reads and parses the configuration file,
 * providing settings for other components.
 * The Server class creates the required server sockets and binds them to the specified ports.
 * Once initialisation is complete, the server enters the main loop.
 * The PollingService class polls the server sockets for events.
 * When an event is detected or the timeout is reached, the EventManager class handles the event.
 * Requests are managed by the RequestHandler class, which utilizes the ClientHandler to communicate with the client,
 * the RequestParser to parse the request, the Router to select the appropriate route and to generate a Response.
 * The generated Response is then pushed to the BufferManager, who will send it to the client in the next cycle
 * if polling indicates the socket is ready.
 * All the while, the Logger class registers errors and access log entries with the BufferManager,
 * who writes them non-blockingly to the log file.
 * This process continues in a loop.
 */

int main(int argc, char **argv)
{
    // Instantiate the bufferManager.
    BufferManager bufferManager;

    // Instantiate the Socket instance.
    Socket socket;

    // Instantiate the logger.
    Logger logger(bufferManager);

    // Instantiate the exceptionHandler.
    ExceptionHandler exceptionHandler(logger);

    try
    {
        // Instantiate the Configuration instance
        Configuration configuration(logger);

        // parse the configuration file

        // Instantiate the PollfdManager.
        PollfdManager pollfdManager(configuration);

        // Configure the logger
        LoggerConfiguration loggerConfiguration(bufferManager, configuration, pollfdManager);
        logger.configure(loggerConfiguration);

        // Instantiate the Factory.
        Factory factory(configuration, logger);

        // Instantiate the SessionManager.
        SessionManager sessionManager(logger, factory);

        // Instantiate the Server.
        Server server(socket, pollfdManager, sessionManager, configuration, logger);

        // Instantiate the Router.
        Router router(configuration, logger);

        // Instantiate the RequestHandler.
        RequestHandler requestHandler(socket, bufferManager, sessionManager, router, logger, exceptionHandler);

        // Instantiate the PollingService.
        PollingService pollingService(pollfdManager, logger);

        // Instantiate the EventManager.
        EventManager eventManager(pollfdManager, bufferManager, socket, server, requestHandler, logger);

        // Start the webserv core cycle.
        while (true)
        {
            try
            {
                // Poll events.
                pollingService.pollEvents();

                // Handle events.
                eventManager.handleEvents();
            }
            catch (WebservException &e)
            {
                // Handle exceptions.
                exceptionHandler.handleException(e, "webserv core cycle: ");
            }
        }
    }
    catch (WebservException &e)
    {
        // Handle setup exceptions.
        exceptionHandler.handleException(e, "webserv setup: ");
    }
}

// Path: main.cpp
