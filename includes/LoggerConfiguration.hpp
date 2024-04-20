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
    const LoggerType _type;
    std::string _logFile;
    IBufferManager &_bufferManager;
    size_t _bufferSize;
    LogLevel _logLevel;
    int _logFileDescriptor;
    bool _enabled;

public:
    LoggerConfiguration(LoggerType type, IBufferManager &BufferManager, IConfiguration &configuration, IPollfdManager &pollfdManager);
    ~LoggerConfiguration();

    virtual int getLogFileDescriptor() const;
    virtual IBufferManager &getBufferManager() const;
    virtual LoggerType getLoggerType() const;
    virtual std::string getLogFile() const;
    virtual LogLevel getLogLevel() const;
    virtual bool getEnabled() const;
};

#endif // LOGGERCONFIGURATION_HPP
        // Path: includes/LoggerConfiguration.hpp
