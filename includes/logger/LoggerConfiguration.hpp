#ifndef LOGGERCONFIGURATION_HPP
#define LOGGERCONFIGURATION_HPP

/*
 * LoggerConfiguration.hpp
 * This class is responsible for configuring the logger module
 *
 * It expects the following configuration directives:
 *   - in the main block: error_log <path> <level>
 *   - in the http block: access_log <path>
 *
 * To disable error logging, set the error_log directive to "off": 'error_log
 * off' To disable access logging, set the access_log directive to "off":
 * 'access_log off'
 *
 * NOTE: supported log levels currently are: verbose, debug, info, warn, error,
 * critical
 */

#define LOG_BUFFER_SIZE 4096 // 4KB
#define DEFAULT_LOG_LEVEL                                                      \
    WARN // Default log level - Delete this line once Configuration default
         // values are implemented

#include "../configuration/BlockList.hpp"
#include "../configuration/IConfiguration.hpp"
#include "../constants/LogLevelHelper.hpp"
#include "../pollfd/IPollfdManager.hpp"
#include "ILoggerConfiguration.hpp"
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
    LogLevelHelper _logLevelHelper;

public:
    LoggerConfiguration(IBufferManager &BufferManager,
                        IConfiguration &configuration,
                        IPollfdManager &pollfdManager);
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
