#include "../includes/ClientHandler.hpp"

/*
 * Summary:
 * ClientHandler class is responsible for handling communication with clients.
 * It reads requests from clients and sends responses back. It also handles errors
 * that occur during communication and logs them appropriately.
 */

// Constructor
ClientHandler::ClientHandler(const ISocket &socket, ILogger &errorLogger, const IExceptionHandler &exceptionHandler)
    : _socket(socket),
      _errorLogger(errorLogger),
      _exceptionHandler(exceptionHandler) {}

// Destructor
ClientHandler::~ClientHandler() {}

// Setter method to set the socket descriptor
void ClientHandler::setSocketDescriptor(int socketDescriptor)
{
    this->_socketDescriptor = socketDescriptor;
}

// Method to read a request from the client
const std::vector<char> ClientHandler::readRequest() const
{
    size_t offSet = 0;
    size_t bufferSize = 4096;
    ssize_t maxReadSize = bufferSize - offSet;
    ssize_t bytesRead = 0;
    std::vector<char> buffer(bufferSize);

    // Read data from the client socket
    // Double the buffer as long as necessary
    while ((bytesRead = this->_socket.recv(this->_socketDescriptor, &buffer[offSet], maxReadSize)) == maxReadSize)
    {
        // Move the offset to the end of the buffer
        offSet += bytesRead;
        // Double the buffer size
        bufferSize *= 2;
        // Calculate the new maximum read size
        maxReadSize = bufferSize - offSet;
        // Resize the buffer
        buffer.resize(bufferSize);
    }

    // Handle read errors
    // NOTE that MSG_DONTWAIT might cause -1 to be returned
    // it is unclear if this is an error (ECONNRESET) or not (EWOULDBLOCK/EAGAIN) without reading errno
    if (bytesRead == -1)
    {
        buffer.clear();
        throw WebservException(ERROR, "Error reading request from client on socket: " + std::to_string(this->_socketDescriptor), 1);
    }

    // Resize the buffer to the actual size of the data read
    buffer.resize(offSet + bytesRead);

    return buffer;
}

// Method to send a response to the client as a vector of characters
ssize_t ClientHandler::sendResponse(const std::vector<char> &response) const
{
    ssize_t bytesSent = this->_socket.send(this->_socketDescriptor, response);
    if (bytesSent == -1)
        this->_errorLogger.errorLog(ERROR, "Error sending response to client on socket: " + std::to_string(this->_socketDescriptor));

    return bytesSent;
}

// Method to send a response to the client as a string
ssize_t ClientHandler::sendResponse(const std::string &response) const
{
    return this->sendResponse(std::vector<char>(response.begin(), response.end()));
}

// Path: includes/ClientHandler.hpp