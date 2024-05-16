#include "includes/configuration/ConfigurationLoader.hpp"
#include "includes/network/Server.hpp"
#include "includes/network/Socket.hpp"
#include "includes/core/PollingService.hpp"
#include "includes/core/EventManager.hpp"
#include "includes/connection/ConnectionManager.hpp"
#include "includes/connection/RequestHandler.hpp"
#include "includes/connection/ClientHandler.hpp"
#include "includes/logger/Logger.hpp"
#include "includes/logger/LoggerConfiguration.hpp"
#include "includes/buffer/BufferManager.hpp"
#include "includes/factory/Factory.hpp"
#include "includes/exception/ExceptionHandler.hpp"
#include "includes/response/Router.hpp"
#include "includes/pollfd/PollfdManager.hpp"

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
    // Get the configuration file path.
    std::string configPath;
    if (argc == 1)
        configPath = "config/default.conf";
    else
        configPath = argv[1];

    // Instantiate the Socket instance.
    Socket socket;

    // Instantiate the bufferManager.
    BufferManager bufferManager(socket);

    // Instantiate the logger.
    Logger logger(bufferManager);

    // Instantiate the Client Handler.
    ClientHandler clientHandler(socket, logger);

    // Instantiate the exceptionHandler.
    ExceptionHandler exceptionHandler(logger);

	ConfigurationLoader	confLoader(logger);

    try
    {

        // load configuration from file and create the configuration object.
        const IConfiguration& configuration = confLoader.loadConfiguration(configPath);

        // parse the configuration file

        // Instantiate the PollfdManager.
        PollfdManager pollfdManager(configuration);

        // Configure the logger
        LoggerConfiguration loggerConfiguration(bufferManager, configuration, pollfdManager);
        logger.configure(loggerConfiguration);

        // Instantiate the Factory.
        Factory factory(configuration, logger);

        // Instantiate the ConnectionManager.
        ConnectionManager connectionManager(logger, factory);

        // Instantiate the Server.
        Server server(socket, pollfdManager, connectionManager, configuration, logger);

        // Instantiate the Router.
        Router router(configuration, logger);

        // Instantiate the RequestHandler.
        RequestHandler requestHandler(bufferManager, connectionManager, configuration, router, logger, exceptionHandler, clientHandler);

        // Instantiate the PollingService.
        PollingService pollingService(pollfdManager, logger);

        // Instantiate the EventManager.
        EventManager eventManager(pollfdManager, bufferManager, server, requestHandler, logger);

        // Start the webserv core cycle.
        while (true)
        {
            try
            {
                // Poll events.
                pollingService.pollEvents();

                // Handle events.
                eventManager.handleEvents();

                // Collect garbage.
                connectionManager.collectGarbage();
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
