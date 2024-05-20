#ifndef MOCKLOGGERCONFIGURATION_HPP
#define MOCKLOGGERCONFIGURATION_HPP

/*
 * MockLoggerConfiguration.hpp
 * Mocks the LoggerConfiguration class
 */

#include "../../includes/buffer/IBufferManager.hpp"
#include "../../includes/configuration/IConfiguration.hpp"
#include "../../includes/logger/ILoggerConfiguration.hpp"
#include "../../includes/pollfd/IPollfdManager.hpp"
#include <unistd.h>

class MockLoggerConfiguration : public ILoggerConfiguration
{
private:
    std::string _errorLogFile;
    std::string _accessLogFile;
    IBufferManager &_bufferManager;
    size_t _bufferSize;
    int _errorLogFileDescriptor;
    int _accessLogFileDescriptor;
    bool _errorLogEnabled;
    bool _accessLogEnabled;

public:
    MockLoggerConfiguration(IBufferManager &bufferManager,
                            const IConfiguration &configuration,
                            IPollfdManager &pollfdManager);

    virtual ~MockLoggerConfiguration();
    virtual void setErrorLogEnabled(bool);
    virtual void setAccessLogEnabled(bool);
    virtual int getErrorLogFileDescriptor() const;
    virtual int getAccessLogFileDescriptor() const;
    virtual IBufferManager &getBufferManager() const;
    virtual std::string getErrorLogFile() const;
    virtual std::string getAccessLogFile() const;
    virtual LogLevel getLogLevel() const;
    virtual bool getErrorLogEnabled() const;
    virtual bool getAccessLogEnabled() const;
    virtual void requestFlush(int descriptor);

    // Testing purposes
    virtual void setBufferSize(size_t bufferSize);
    virtual void setFileDescriptor(int errorLogFileDescriptor,
                                   int accessLogFileDescriptor = STDOUT_FILENO);
};

#endif // MOCKLOGGERCONFIGURATION_HPP
       // Path: tests/mock_includes/MockLoggerConfiguration.hpp
