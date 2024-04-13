#ifndef IEXCEPTIONHANDLER_HPP
#define IEXCEPTIONHANDLER_HPP

#include <string>
#include <exception>

class IExceptionHandler
{
public:
    virtual ~IExceptionHandler() {}

    // Method to handle exceptions
    virtual int handleException(const std::exception &e, const std::string &context="") const = 0;
};

#endif // IEXCEPTIONHANDLER_HPP
// Path: includes/ILogger.hpp