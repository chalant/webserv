#ifndef MOCKCONFIGURATION_HPP
#define MOCKCONFIGURATION_HPP

/*
 * MockConfiguration class is a mock implementation of the IConfiguration interface
 * This mock implementation is used for testing purposes to simulate the behavior of the IConfiguration interface
 * during unit testing without relying on the actual implementation.
*/

#include "../../includes/IConfiguration.hpp" // Include the base interface header

class MockConfiguration : public IConfiguration
{
public:
    // Constructor
    MockConfiguration();

    // Destructor
    virtual ~MockConfiguration();

    // Methods implementing the IConfiguration interface
    // These methods are overridden to provide custom behavior for testing.

    // Getters
    virtual size_t getClientHeaderBufferSize() const;
    virtual size_t getClientBodyBufferSize() const;
    virtual size_t getClientMaxBodySize() const;
    virtual size_t getClientMaxUriSize() const;
    virtual size_t getLogBufferSize() const;
    virtual size_t getMaxConnections() const;
    virtual int getPort() const;
    virtual LogLevel getLogLevel() const;
    virtual bool getErrorLogEnabled() const;
    virtual bool getAccessLogEnabled() const;
    virtual std::string getErrorLogFile() const;
    virtual std::string getAccessLogFile() const;
    virtual std::set<std::string> getSupportedMethods() const;
};

#endif // MOCKCONFIGURATION_HPP
// Path: tests/mock_headers/MockLogger.hpp
