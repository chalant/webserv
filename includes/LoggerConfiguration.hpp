#ifndef LOGGERCONFIGURATION_HPP
#define LOGGERCONFIGURATION_HPP

#include "ILoggerConfiguration.hpp"
#include "IConfiguration.hpp"
#include "IBufferManager.hpp"
#include "IPollfdManager.hpp"
#include "WebservExceptions.hpp"
#include <fcntl.h>

class LoggerConfiguration : public ILoggerConfiguration
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
    LoggerConfiguration(IBufferManager &BufferManager, IConfiguration &configuration, IPollfdManager &pollfdManager);
    ~LoggerConfiguration();

    virtual int getErrorLogFileDescriptor() const;
    virtual int getAccessLogFileDescriptor() const;
    virtual IBufferManager &getBufferManager() const;
    virtual std::string getErrorLogFile() const;
    virtual std::string getAccessLogFile() const;
    virtual LogLevel getLogLevel() const;
    virtual bool getErrorLogEnabled() const;
    virtual bool getAccessLogEnabled() const;
    virtual void requestFlush(int descriptor);
};

#endif // LOGGERCONFIGURATION_HPP
       // Path: includes/LoggerConfiguration.hpp
