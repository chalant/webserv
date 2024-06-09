#include "../../includes/connection/RequestHandler.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <unistd.h>
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
    : m_buffer_manager(buffer_manager),
      m_connection_manager(connection_manager),
      m_client_handler(client_handler), m_request_parser(configuration, logger),
      m_router(router), m_http_helper(configuration), m_logger(logger),
      m_exception_handler(exception_handler)
{
    // Log the creation of the RequestHandler instance.
    m_logger.log(VERBOSE, "RequestHandler instance created.");
}

#include <iostream>
// Destructor
RequestHandler::~RequestHandler()
{
    // Log the destruction of the RequestHandler instance.
    m_logger.log(VERBOSE, "RequestHandler instance destroyed.");
}
#include <iostream>
// Handles a client request
// Returns Cgi Info for dynamic content or -1 for static content and invalid
// requests
Triplet_t RequestHandler::handleRequest(int socket_descriptor)
{
    // Give the 'ClientHandler' the current socket descriptor
    m_client_handler.setSocketDescriptor(socket_descriptor);

    // Get a reference to the Connection
    IConnection &connection =
        m_connection_manager.getConnection(socket_descriptor);

    // Update the connection's last activity time
    connection.touch();

    // Get a reference to the Request
    IRequest &request = connection.getRequest();

    // Get a reference to the Response
    IResponse &response = connection.getResponse();

    RequestState &state = request.getState();

    try
    {
        // Read the raw request from the client
        std::vector<char> raw_request = m_client_handler.readRequest();

        // Check if the client has disconnected
        if (raw_request.empty())
        {
            // EOF read, indicating an orderly disconnect
            return Triplet_t(-3, std::pair<int, int>(-1, -1));
        }

        // Append the raw request to the request buffer
        request.appendBuffer(raw_request);

        if (state.initial())
        {
            // If raw request contains CRLF CRLF, we move to the next stage
            // CRLF CRLF (\r\n\r\n) marks the end of the headers
            std::vector<char> buffer = request.getBuffer();
            std::string buffer_str(buffer.begin(), buffer.end());
            if (buffer_str.find("\r\n\r\n") != std::string::npos)
            {
                state.initial(false);
                state.headers(true); // Because we now have all the headers
            }
            else
            {
                // log the situation
                m_logger.log(VERBOSE,
                             "RequestHandler::handleRequest: Request is "
                             "incomplete - state: initial. Buffer: " +
                                 buffer_str.substr(0, 10));
                return Triplet_t(-2, std::pair<int, int>(-1, -1));
            }
        }
        if (state.headers()) // Parse the headers etc.
        {
            m_request_parser.parseRequest(request);
            state.headers(false);
            // Assign session to connection
            m_connection_manager.assignSessionToConnection(connection, request,
                                                           response);
            if (!state.finished())
            {
                // log the situation
                m_logger.log(VERBOSE, "RequestHandler::handleRequest: Request "
                                      "is incomplete - state: headers done.");
                return Triplet_t(-2, std::pair<int, int>(-1, -1));
            }
        }
        else if (!state.finished())
        {
            m_request_parser.parseBody(request);
            if (!state.finished())
            {
                // log the situation
                m_logger.log(VERBOSE, "RequestHandler::handleRequest: Request "
                                      "is incomplete - state: partial body.");
                return Triplet_t(-2, std::pair<int, int>(-1, -1));
            }
        }
        // Delete these 3 lines once router is implemented
        //(void)m_router;
        // Triplet_t cgi_info(-1, std::pair<int, int>(-1, -1));
        // throw HttpStatusCodeException(
        //   NOT_IMPLEMENTED,
        //   "RequestHandler::handleRequest: Router not implemented yet.");
        state.setRoute(m_router.getRoute(&request, &response));
        // todo: Route the request, return the CGI info
        //Triplet_t cgi_info = m_router.execRoute(&request, &response);
		Triplet_t cgi_info = m_router.execRoute(state.getRoute(), &request, &response);
		state.reset();

        // If dynamic content is being created, return the info
        if (cgi_info.first != -1)
        {
            // Get CGI Info
            int cgi_pid = cgi_info.first;
            int cgi_output_pipe_read_end = cgi_info.second.first;
            int cgi_input_pipe_write_end = cgi_info.second.second;

            // Record the cgi info
            connection.setCgiInfo(cgi_pid, cgi_output_pipe_read_end,
                                  cgi_input_pipe_write_end);

            // Record the pipes to connection socket mappings
            m_pipe_routes[ cgi_output_pipe_read_end ] = socket_descriptor;
            m_pipe_routes[ cgi_input_pipe_write_end ] = socket_descriptor;

            // Push the request body to the request pipe
            m_buffer_manager.pushFileBuffer(cgi_input_pipe_write_end,
                                            request.getBody());

            return cgi_info; // cgi content
        }
        else // static content
        {
            // Push the response to the buffer
            m_sendResponse(socket_descriptor);
            return Triplet_t(-1, std::pair<int, int>(-1, -1));
        }
    }

    catch (const WebservException &e)
    {
        // Set the request state to finished
        state.finished(true);
        
        // Get the status code
        int status_code;
        if (dynamic_cast<const HttpStatusCodeException *>(&e))
            status_code =
                e.getErrorCode(); // An HttpStatusCodeException was thrown
        else
            status_code = 500; // Internal Server Error; Default status code for
                               // other exceptions

        // Log the exception
        m_exception_handler.handleException(
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
    int client_socket = m_pipe_routes[ pipe_descriptor ];

    // Remove the pipe descriptor from the map
    m_pipe_routes.erase(pipe_descriptor);

    // Handle error response
    this->handleErrorResponse(client_socket, INTERNAL_SERVER_ERROR);

    // Return the client socket descriptor
    return client_socket;
}

// Handles read input from pipe
// Read into the response buffer until the pipe is empty or blocks
// returns the client socket descriptor destination for the response
// or -1 in case of blocking
int RequestHandler::handlePipeRead(int cgi_output_pipe_read_end)
{
    // Get the client socket descriptor linked to the pipe
    int client_socket = m_pipe_routes[ cgi_output_pipe_read_end ];

    // Give the ClientHandler the current socket descriptor
    m_client_handler.setSocketDescriptor(client_socket);

    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(client_socket);

    // Get a reference to the Response Buffer
    std::vector<char> &response_buffer = response.getBuffer();

    // Read the response from the pipe
    size_t read_buffer_size = 8;
    ssize_t read_return_value;

    // Read the response from the pipe until the pipe is empty or blocks
    do
    {
        std::cout << "Response buffer size: " << response_buffer.size() << std::endl;

        // Resize the response buffer
        response_buffer.resize(response_buffer.size() + read_buffer_size);

        // Read the response from the pipe
        read_return_value = read(cgi_output_pipe_read_end, response_buffer.data() + response_buffer.size() - read_buffer_size,
                         read_buffer_size);

        std::cout << "Read return value: " << read_return_value << std::endl;
    } while (read_return_value == static_cast<ssize_t>(read_buffer_size));

    // Handle blocking read
    if (read_return_value < 0)
    {
        // Resize the response buffer to the actual size
        response_buffer.resize(response_buffer.size() - read_buffer_size);

        // test print the raw_response content
        std::cout << " Read returned < 0; errno: " << errno << std::endl;
        std::string raw_response_str(response_buffer.begin(), response_buffer.end());
        std::cout << "Current response buffer content: " << raw_response_str << std::endl;

        // return -1 to indicate that we are not done reading
        return -1;
    }

    // Resize the response buffer to the actual size
    response_buffer.resize(response_buffer.size() - read_buffer_size + read_return_value);

    // print the response
    std::string raw_response_str(response_buffer.begin(), response_buffer.end());
    m_logger.log(VERBOSE, "RequestHandler::handlePipeRead: Response from CGI: " +
                              raw_response_str);

    // Set the response
    if (response_buffer.empty()) // Check if the response is empty
        response.setErrorResponse(INTERNAL_SERVER_ERROR); // 500
    else
        response.setCgiResponse(response_buffer); // Good response

    // Push the response to the buffer
    m_sendResponse(client_socket);

    // Clean up

    // Get a reference to the Connection
    IConnection &connection = m_connection_manager.getConnection(client_socket);

    // Get the CGI input pipe write end
    int cgi_input_pipe_write_end = connection.getCgiInputPipeWriteEnd();

    // Remove the descriptors from the pipe;socket map
    m_pipe_routes.erase(cgi_output_pipe_read_end);
    m_pipe_routes.erase(cgi_input_pipe_write_end);

    // Close the pipes
    close(cgi_output_pipe_read_end);
    close(cgi_input_pipe_write_end);

    // Return the client socket descriptor
    return client_socket;
}

// Sends the response to the buffer
int RequestHandler::m_sendResponse(int socket_descriptor)
{
    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(socket_descriptor);

    // Serialise the response
    std::vector<char> serialised_response = response.serialise();

    // Push the response to the buffer
    m_buffer_manager.pushSocketBuffer(socket_descriptor, serialised_response);

    // create an access log entry
    m_logger.log(m_connection_manager.getConnection(socket_descriptor));

    // return 0
    return (0);
}

// Handles error responses
void RequestHandler::handleErrorResponse(int socket_descriptor, int status_code)
{
    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(socket_descriptor);

    // Set the response to the error status code
    response.setErrorResponse(status_code);

    // Push the response to the buffer
    m_sendResponse(socket_descriptor);
}

// Handle error responses - HttpStatusCode input
void RequestHandler::handleErrorResponse(int socket_descriptor,
                                         HttpStatusCode status_code)
{
    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(socket_descriptor);

    // Set the response to the error status code
    response.setErrorResponse(status_code);

    // Push the response to the buffer
    m_sendResponse(socket_descriptor);
}

// Remove and close the connection
void RequestHandler::removeConnection(int socket_descriptor)
{
    // Get a reference to the Connection
    IConnection &connection =
        m_connection_manager.getConnection(socket_descriptor);

    // Get a reference to the Request
    IRequest &request = connection.getRequest();

    // Remove the connection if the 'Connection' header is set to 'close'
    if (request.getHeaderValue(CONNECTION) == "close")
        m_connection_manager.removeConnection(socket_descriptor);
    else // temp fix; close it anyway
        m_connection_manager.removeConnection(socket_descriptor);

}

// path: srcs/RequestHandler.cpp
