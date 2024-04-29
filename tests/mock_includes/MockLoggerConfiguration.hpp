#ifndef MOCKLOGGERCONFIGURATION_HPP
#define MOCKLOGGERCONFIGURATION_HPP

/*
 * MockLoggerConfiguration.hpp
 * Mocks the LoggerConfiguration class
 */

#include "logger/ILoggerConfiguration.hpp"
#include "buffer/IBufferManager.hpp"
#include "pollfd/IPollfdManager.hpp"
#include "configuration/IConfiguration.hpp"

class MockLoggerConfiguration : public ILoggerConfiguration
{
private:
    std::string _errorLogFile;
    std::string _accessLogFile;
    IBufferManager &_bufferManager;
    IPollfdManager &_pollfdManager;
    size_t _bufferSize;
    LogLevel _logLevel;
    int _errorLogFileDescriptor;
    int _accessLogFileDescriptor;
    bool _errorLogEnabled;
    bool _accessLogEnabled;
public:
    MockLoggerConfiguration(IBufferManager &bufferManager, IConfiguration &configuration, IPollfdManager &pollfdManager);

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
    virtual void setFileDescriptor(int errorLogFileDescriptor, int accessLogFileDescriptor=STDOUT_FILENO);
};

#endif // MOCKLOGGERCONFIGURATION_HPP
       // Path: tests/mock_includes/MockLoggerConfiguration.hpp