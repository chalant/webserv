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
    : m_buffer_manager(buffer_manager), m_connection_manager(connection_manager),
      m_client_handler(client_handler), m_request_parser(configuration, logger),
      m_router(router), m_http_helper(configuration), m_logger(logger),
      m_exception_handler(exception_handler)
{
    // Log the creation of the RequestHandler instance.
    m_logger.log(VERBOSE, "RequestHandler instance created.");
}

// Destructor
RequestHandler::~RequestHandler()
{
    // Log the destruction of the RequestHandler instance.
    m_logger.log(VERBOSE, "RequestHandler instance destroyed.");
}

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

	RequestState	&state = request.getState();

    try
    {
        // Read the raw request from the client
        std::vector<char>	raw_request = m_client_handler.readRequest();
		// Assign session to connection
		//m_request_parser.parseRequest(raw_request, request);
        m_connection_manager.assignSessionToConnection(connection, request,
                    	                                     response);
		if (state.initial())
		{
			m_request_parser.parseRequestHeader(raw_request, request);
			state.initial(false);
			if (!state.finished())
			{
				return Triplet_t(-2, std::pair<int, int>(-1, -1));
			}
		}
		else if (!state.finished())
		{
			m_request_parser.parsePartialBody(raw_request, request);
			if (!state.finished())
			{
				return Triplet_t(-2, std::pair<int, int>(-1, -1));
			}
		}
		//finally, parse body parameters
		m_request_parser.parseBodyParameters(request);
		state.reset();
        
		// Parse the raw request into a Request object
        //m_request_parser.parseRequest(raw_request, request);

        // Delete these 3 lines once router is implemented
        //(void)m_router;
        // Triplet_t cgi_info(-1, std::pair<int, int>(-1, -1));
        // throw HttpStatusCodeException(
        //   NOT_IMPLEMENTED,
        //   "RequestHandler::handleRequest: Router not implemented yet.");

        // todo: Route the request, return the CGI info
        Triplet_t cgi_info = m_router.execRoute(&request, &response);

        // If dynamic content is being created, return the info
        if (cgi_info.first != -1)
        {
            // Get CGI Info
            int cgi_pid = cgi_info.first;
            int cgi_output_pipe_read_end = cgi_info.second.first;
            int cgi_input_pipe_write_end = cgi_info.second.second;

            // Record the cgi info
            connection.setCgiInfo(cgi_pid, cgi_output_pipe_read_end, cgi_input_pipe_write_end);

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
			request.getBody().clear();
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
#include <iostream>
// Handles read input from pipe - responses larger than the buffer size are
// currently not supported
int RequestHandler::handlePipeRead(int cgi_output_pipe_read_end)
{
    std::cout << "handlePipeRead" << std::endl;
    // Get the client socket descriptor linked to the pipe
    int client_socket = m_pipe_routes[ cgi_output_pipe_read_end ];

    // Give the ClientHandler the current socket descriptor
    m_client_handler.setSocketDescriptor(client_socket);

    // Read the response from the pipe
    std::vector<char> raw_response(4096, 0);
    ssize_t bytesread = read(cgi_output_pipe_read_end, raw_response.data(), raw_response.size());
    if (bytesread < 0)
    {
        // Handle error response
        this->handleErrorResponse(client_socket, INTERNAL_SERVER_ERROR);
        return client_socket;
    }
    raw_response.resize(bytesread);

    std::cerr << "Response: " << raw_response.data() << "END OF RESPONSE" << std::endl;

    // Get a reference to the Response
    IResponse &response = m_connection_manager.getResponse(client_socket);

    // Set the response
    if (raw_response.empty()) // Check if the response is empty
        response.setErrorResponse(INTERNAL_SERVER_ERROR); // 500
    else
        response.setCgiResponse(raw_response); // Good response

    // Push the response to the buffer
    m_sendResponse(client_socket);

    // Clean up

    // Get a reference to the Connection
    IConnection &connection =
        m_connection_manager.getConnection(client_socket);

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
    IResponse &response =
        m_connection_manager.getResponse(socket_descriptor);
    // Get a reference to the Request
    IRequest &request =
        m_connection_manager.getConnection(socket_descriptor).getRequest();
    // Serialise the response
    std::vector<char> serialised_response = response.serialise();
    // Push the response to the buffer
    m_buffer_manager.pushSocketBuffer(socket_descriptor, serialised_response);

    // create an access log entry
    m_logger.log(m_connection_manager.getConnection(socket_descriptor));

    // Remove the connection if the 'Connection' header is set to 'close'
    if (request.getHeaderValue(CONNECTION) == "close")
        m_connection_manager.removeConnection(socket_descriptor);

    // return 0
    return (0);
}

// Handles error responses
void RequestHandler::handleErrorResponse(int socket_descriptor, int status_code)
{
    // Get a reference to the Response
    IResponse &response =
        m_connection_manager.getResponse(socket_descriptor);

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
    IResponse &response =
        m_connection_manager.getResponse(socket_descriptor);

    // Set the response to the error status code
    response.setErrorResponse(status_code);

    // Push the response to the buffer
    m_sendResponse(socket_descriptor);
}

// path: srcs/RequestHandler.cpp
