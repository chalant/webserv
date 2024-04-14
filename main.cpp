#include "includes/IConfiguration.hpp"
#include "includes/Server.hpp"
#include "includes/Router.hpp"
#include "includes/IClientHandler.hpp"
#include "includes/RequestParser.hpp"
#include "includes/ARequestHandler.hpp"
#include "includes/Response.hpp"
#include "includes/ILogger.hpp"
#include "includes/Logger.hpp"
#include "includes/IExceptionHandler.hpp"
#include "includes/ExceptionHandler.hpp"
#include "includes/Socket.hpp"
#include "includes/Sessions.hpp"

/*
 * webserv Workflow:
 *
 * Upon launch, the configuration class reads and parses the configuration file,
 * providing settings for other components.
 * The Server class sets up a TCP connection, monitors events using a poll mechanism,
 * and accepts incoming connections.
 * When an event is detected by the poll mechanism, the Sessions class coordinates request processing.
 * It utilizes the ClientHandler to communicate with the client,
 * the RequestParser to parse the request, the Router to select the appropriate ARequestHandler,
 * and finally, the ARequestHandler to generate a Response.
 * The generated Response is then sent back to the client by the ClientHandler.
 * After processing a request, the ILogger class writes the log buffers to files.
 * This process continues in a loop.
 */

int main(int argc, char **argv)
{
    // Instantiate the errorLogger.
    Logger errorLogger;
    // Instantiate the exceptionHandler.
    ExceptionHandler exceptionHandler(errorLogger);

    try
    {
        // Instantiate the IConfiguration instance. Verifies, reads, parses, and stores the config file data.
        Configuration configuration(argc, argv, errorLogger, exceptionHandler);
        // Instantiate the PollfdManager. Manages the pollfd array
        PollfdManager pollfdManager(configuration.getMaxConnections());
        // Re-instantiate the errorLogger to apply configuration.
        //errorLogger Logger(ERRORLOGGER, configuration, pollfdManager);
        // Instantiate the accessLogger, primarily for use by the RequestHandler class to handle access event logging.
        Logger accessLogger(ACCESSLOGGER, configuration, pollfdManager);
        // Instantiate the Socket instance. Provides utility functions for socket operations.
        Socket socket;
        // Instantiate the Server. Sets up connectivity, responsible for polling and accepting connections.
        Server server(socket, pollfdManager, configuration, errorLogger, accessLogger, exceptionHandler);
        // Instantiate the Sessions. Coordinates request processing utilizing the poll fd array.
        Sessions sessions(socket, pollfdManager, configuration, errorLogger, accessLogger, exceptionHandler);

        // Start the webserv core cycle.
        while (true)
        {
            try
            {
                // Poll events in a non-blocking way.
                server.pollEvents();
                // Accept new connections.
                server.acceptConnection();
                // Process events.
                sessions.processEvents();
            }
            catch (WebservException &e)
            {
                exceptionHandler.handleException(e, "webserv core cycle: ");
            }
        }
    }
    catch (WebservException &e)
    {
        exceptionHandler.handleException(e, "webserv setup: ");
    }
}
