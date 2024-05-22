#include "../../includes/connection/RequestHandler.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <utility>

/*
 * RequestHandler class
 *
 * Responsible for handling client requests:
 * - Reads the raw request from the client
 * - Parses it into a Request object
 * - Passes the Request object to the Router
 *
 * The response is either served statically or dynamically, depending on the
 * route:
 * - Served statically: Response sent immediately to buffer
 * - Served dynamically: Response obtained from a separate process
 *   (read end of the pipe is returned)
 */

// Constructor
RequestHandler::RequestHandler(IBufferManager &buffer_manager,
                               IConnectionManager &connection_manager,
                               const IConfiguration &configuration,
                               ITempRouter &router, ILogger &logger,
                               const IExceptionHandler &exception_handler,
                               IClientHandler &client_handler)
    : m_buffer_manager(buffer_manager), m_connection_manager(connection_manager),
      m_client_handler(client_handler), m_request_parser(configuration, logger),
      m_router(router), m_http_helper(), m_logger(logger),
      m_exception_handler(exception_handler)
{
    // Log the creation of the RequestHandler instance.
    this->m_logger.log(VERBOSE, "RequestHandler instance created.");
}

// Destructor
RequestHandler::~RequestHandler()
{
    // Log the destruction of the RequestHandler instance.
    this->m_logger.log(VERBOSE, "RequestHandler instance destroyed.");
}

// Handles a client request
// Returns Cgi Info for dynamic content or -1 for static content and invalid
// requests
Triplet_t RequestHandler::handleRequest(int socket_descriptor)
{
    // Give the 'ClientHandler' the current socket descriptor
    this->m_client_handler.setSocketDescriptor(socket_descriptor);

    // Get a reference to the Connection
    IConnection &connection =
        this->m_connection_manager.getConnection(socket_descriptor);

    // Update the connection's last activity time
    connection.touch();

    // Get a reference to the Request
    IRequest &request = connection.getRequest();

    // Get a reference to the Response
    IResponse &response = connection.getResponse();

    try
    {
        // Read the raw request from the client
        std::vector<char> raw_request = this->m_client_handler.readRequest();

        // Parse the raw request into a Request object
        this->m_request_parser.parseRequest(raw_request, request);

        // Assign session to connection
        this->m_connection_manager.assignSessionToConnection(connection, request,
                                                           response);

        // Delete these 3 lines once router is implemented
        //(void)this->m_router;
        // Triplet_t cgi_info(-1, std::pair<int, int>(-1, -1));
        // throw HttpStatusCodeException(
        //   NOT_IMPLEMENTED,
        //   "RequestHandler::handleRequest: Router not implemented yet.");

        // todo: Route the request, return the CGI info
        Triplet_t cgi_info = this->m_router.execRoute(&request, &response);

        // If dynamic content is being created, return the info
        if (cgi_info.first != -1)
        {
            // Get CGI Info
            int cgi_pid = cgi_info.first;
            int response_read_pipe = cgi_info.second.first;
            int request_write_pipe = cgi_info.second.second;

            // Record the cgi info
            connection.setCgiInfo(cgi_pid, response_read_pipe, request_write_pipe);

            // Record the pipes to connection socket mappings
            this->m_pipe_routes[ response_read_pipe ] = socket_descriptor;
            this->m_pipe_routes[ request_write_pipe ] = socket_descriptor;

            // Push the request body to the request pipe
            this->m_buffer_manager.pushSocketBuffer(request_write_pipe,
                                                  request.getBody());

            return cgi_info; // cgi content
        }
        else // static content
        {
            // Push the response to the buffer
            this->m_sendResponse(socket_descriptor);
            // return -1
            return Triplet_t(-1, std::pair<int, int>(-1, -1));
        }
    }

    catch (const WebservException &e)
    {
        // Get the status code
        int status_code;
        if (dynamic_cast<const HttpStatusCodeException *>(&e))
            status_code =
                e.getErrorCode(); // An HttpStatusCodeException was thrown
        else
            status_code = 500; // Internal Server Error; Default status code for
                              // other exceptions

        // Log the exception
        this->m_exception_handler.handleException(
            e, "RequestHandler::processRequest socket=\"" +
                   Converter::toString(socket_descriptor) + "\"");

        // Handle error response
        this->handleErrorResponse(socket_descriptor, status_code);

        // return -1
        return Triplet_t(-1, std::pair<int, int>(-1, -1));
    }
}

// Handles exceptions related to pipe events - returns the client socket
// descriptor destination for the response
int RequestHandler::handlePipeException(int pipe_descriptor)
{
    // Get the client socket descriptor linked to the pipe
    int client_socket = this->m_pipe_routes[ pipe_descriptor ];

    // Remove the pipe descriptor from the map
    this->m_pipe_routes.erase(pipe_descriptor);

    // Handle error response
    this->handleErrorResponse(client_socket, INTERNAL_SERVER_ERROR);

    // Return the client socket descriptor
    return client_socket;
}

// Handles read input from pipe - responses larger than the buffer size are
// currently not supported
int RequestHandler::handlePipeRead(int pipe_descriptor)
{
    // Get the client socket descriptor linked to the pipe
    int client_socket = this->m_pipe_routes[ pipe_descriptor ];

    // Remove the pipe descriptor from the map
    this->m_pipe_routes.erase(pipe_descriptor);

    // Give the ClientHandler the current socket descriptor
    this->m_client_handler.setSocketDescriptor(client_socket);

    // Read the response from the pipe
    std::vector<char> raw_response = this->m_client_handler.readRequest();

    // Get a reference to the Response
    IResponse &response = this->m_connection_manager.getResponse(client_socket);

    // Set the response
    if (raw_response.empty()) // Check if the response is empty
        response.setErrorResponse(INTERNAL_SERVER_ERROR); // 500
    else
        response.setResponse(raw_response); // Good response

    // Push the response to the buffer
    this->m_sendResponse(client_socket);

    // Return the client socket descriptor
    return client_socket;
}

// Sends the response to the buffer
int RequestHandler::m_sendResponse(int socket_descriptor)
{
    // Get a reference to the Response
    IResponse &response =
        this->m_connection_manager.getResponse(socket_descriptor);
    // Get a reference to the Request
    IRequest &request =
        this->m_connection_manager.getConnection(socket_descriptor).getRequest();
    // Serialise the response
    std::vector<char> serialised_response = response.serialise();
    // Push the response to the buffer
    this->m_buffer_manager.pushSocketBuffer(socket_descriptor, serialised_response);

    // create an access log entry
    this->m_logger.log(this->m_connection_manager.getConnection(socket_descriptor));

    // Remove the connection if the 'Connection' header is set to 'close'
    if (request.getHeaderValue(CONNECTION) == "close")
        this->m_connection_manager.removeConnection(socket_descriptor);

    // return 0
    return (0);
}

// Handles error responses
void RequestHandler::handleErrorResponse(int socket_descriptor, int status_code)
{
    // Get a reference to the Response
    IResponse &response =
        this->m_connection_manager.getResponse(socket_descriptor);

    // Set the response to the error status code
    response.setErrorResponse(status_code);

    // Push the response to the buffer
    this->m_sendResponse(socket_descriptor);
}

// Handle error responses - HttpStatusCode input
void RequestHandler::handleErrorResponse(int socket_descriptor,
                                         HttpStatusCode status_code)
{
    // Get a reference to the Response
    IResponse &response =
        this->m_connection_manager.getResponse(socket_descriptor);

    // Set the response to the error status code
    response.setErrorResponse(status_code);

    // Push the response to the buffer
    this->m_sendResponse(socket_descriptor);
}

// path: srcs/RequestHandler.cpp
