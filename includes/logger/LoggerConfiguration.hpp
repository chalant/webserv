#ifndef LOGGERCONFIGURATION_HPP
#define LOGGERCONFIGURATION_HPP

#define LOG_BUFFER_SIZE 4096 // 4KB
#define LOG_LEVEL VERBOSE

#include "ILoggerConfiguration.hpp"
#include <fcntl.h>
#include "../configuration/IConfiguration.hpp"
#include "../pollfd/IPollfdManager.hpp"

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
    LoggerConfiguration(IBufferManager &BufferManager, const IConfiguration &configuration, IPollfdManager &pollfdManager);
    ~LoggerConfiguration();

    virtual void setErrorLogEnabled(bool enabled);
    virtual void setAccessLogEnabled(bool enabled);
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
