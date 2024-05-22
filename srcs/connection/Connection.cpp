#include "../../includes/connection/Connection.hpp"
#include "../../includes/utils/Converter.hpp"
#include <ctime>

/*
 * Connection class
 *
 * This class represents a network connection and interaction with a client.
 * It stores the socket descriptor, client IP address, port number, and remote
 * address, as well as pointers to the request and response objects. Its
 * lifetime is determined either by the 'Connection' header in the request or
 * the timeout value.
 *
 * It is owned by the ConnectionManager.
 *
 */

// Constructor
Connection::Connection(
    std::pair<int, std::pair<std::string, std::string> > client_info,
    ILogger &logger, IRequest *request, IResponse *response, time_t timeout)
    : m_socket_descriptor(client_info.first), m_ip(client_info.second.first),
      m_port(Converter::toInt(client_info.second.second)),
      m_remote_address(m_ip + ":" + client_info.second.second), m_logger(logger),
      m_request(request), m_response(response), m_timeout(timeout)
{
    this->m_last_access = time(NULL);
}

// Destructor
Connection::~Connection()
{
    delete this->m_request;
    delete this->m_response;
}

// Set session
void Connection::setSession(ISession *session) { this->m_session = session; }

// Getters
int Connection::getSocketDescriptor() const { return this->m_socket_descriptor; }

std::string Connection::getIp() const { return this->m_ip; }

int Connection::getPort() const { return this->m_port; }

std::string Connection::getRemoteAddress() const
{
    return this->m_remote_address;
}

int Connection::getResponseReadPipeFd() const
{
    return this->m_response_read_pipe_fd;
}

IRequest &Connection::getRequest() const { return *this->m_request; }

IResponse &Connection::getResponse() const { return *this->m_response; }

ISession &Connection::getSession() const { return *this->m_session; }

void Connection::setCgiInfo(int cgi_pid, int response_read_pipe_fd,
                            int request_write_pipe_fd)
{
    this->m_cgi_pid = cgi_pid;
    this->m_response_read_pipe_fd = response_read_pipe_fd;
    this->m_request_write_pipe_fd = request_write_pipe_fd;
}

// Connection management
void Connection::touch()
{
    // Get the current time
    time_t now = time(NULL);

    // Set time strings
    // char *nowString = ctime(&now);
    // char *thenString = ctime(&this->m_last_access);

    // this->m_logger.log(VERBOSE, "Updating last access for session: " +
    // Converter::toString(this->m_session->getId()) + " from " + thenString + "
    // to " + nowString);

    // Log the last access update
    this->m_logger.log(
        VERBOSE,
        "Updating last access for connection with: " + this->m_remote_address +
            " from " + ctime(&this->m_last_access) + " to " + ctime(&now));

    // Update the last access time
    this->m_last_access = now;
}

bool Connection::hasExpired() const
{
    return time(NULL) - this->m_last_access > this->m_timeout;
}

// Path: srcs/Connection.cpp
