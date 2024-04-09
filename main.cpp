#include "includes/Configuration.hpp"
#include "includes/Server.hpp"
#include "includes/Router.hpp"
#include "includes/ClientHandler.hpp"
#include "includes/RequestParser.hpp"
#include "includes/ARequestHandler.hpp"
#include "includes/Response.hpp"
#include "includes/Logger.hpp"
#include "includes/ExceptionHandler.hpp"
#include "includes/Sessions.hpp"

/*
 * webserv Workflow:
 *
 * Upon launch, the Configuration class reads and parses the configuration file,
 * providing settings for other components.
 * The Server class sets up a TCP connection, monitors events using a poll mechanism,
 * and accepts incoming connections.
 * When an event is detected by the poll mechanism, the Sessions class coordinates request processing.
 * It utilizes the ClientHandler to communicate with the client,
 * the RequestParser to parse the request, the Router to select the appropriate ARequestHandler,
 * and finally, the ARequestHandler to generate a Response.
 * The generated Response is then sent back to the client by the ClientHandler.
 * After processing a request, the Logger class writes the log buffers to files.
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
        // Instantiate the Configuration instance. Verifies, reads, parses, and stores the config file data.
        Configuration configuration(argc, argv, errorLogger, exceptionHandler);
        // Re-instantiate the errorLogger to apply configuration.
        errorLogger = Logger(ERRORLOGGER, configuration);
        // Instantiate the accessLogger, primarily for use by the RequestHandler class to handle access event logging.
        Logger accessLogger(ACCESSLOGGER, configuration);
        // Instantiate the Server. Sets up connectivity, responsible for polling and accepting connections.
        Server server(configuration, errorLogger, exceptionHandler);
        // Instantiate the Sessions. Coordinates request processing utilizing the poll fd array.
        Sessions sessions(configuration, errorLogger, accessLogger, exceptionHandler, server);

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
                // Write error logs.
                errorLogger.writeLogBufferToFile();
                // Write access logs.
                accessLogger.writeLogBufferToFile();
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
