#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

/*
 * Summary:
 * ClientHandler class is responsible for handling communication with clients.
 * It reads requests from clients and sends responses back. It also handles errors
 * that occur during communication and logs them appropriately.
 */

#include <string>
#include "Logger.hpp"
#include "ExceptionHandler.hpp"
#include "Response.hpp"

class ClientHandler
{
private:
    int _socketDescriptor;               // Socket descriptor for client connection
    Logger &_errorLogger;                // Reference to the error logger object
    ExceptionHandler &_exceptionHandler; // Reference to the exception handler object

public:
    // Constructor: Initializes the ClientHandler with an error logger and exception handler
    ClientHandler(Logger &errorLogger, ExceptionHandler &exceptionHandler);

    // Destructor
    ~ClientHandler();

    // Setter method to set the socket descriptor
    void setSocketDescriptor(int socketDescriptor);

    // Method to read a request from the client
    const std::vector<char> readRequest() const;

    // Method to send a response to the client as a vector of characters
    ssize_t sendResponse(const std::vector<char> &response) const;

    // Method to send a response to the client as a string
    ssize_t sendResponse(const std::string &response) const;
};

#endif // CLIENTHANDLER_HPP
       // Path: includes/ClientHandler.hpp