#ifndef ILOGGERCONFIGURATION_HPP
#define ILOGGERCONFIGURATION_HPP

enum LoggerType
{
    ERRORLOGGER,
    ACCESSLOGGER
};

#include "constants/LogLevelHelper.hpp"
#include "IBufferManager.hpp"

#include <string>

class ILoggerConfiguration
{
public:
    virtual ~ILoggerConfiguration() {}

    virtual int getLogFileDescriptor() const = 0;
    virtual IBufferManager &getBufferManager() const = 0;
    virtual LoggerType getLoggerType() const = 0;
    virtual std::string getLogFile() const = 0;
    virtual LogLevel getLogLevel() const = 0;
    virtual bool getEnabled() const = 0;
};

#endif // ILOGGERCONFIGURATION_HPP
// Path: includes/ILoggerConfiguration.hpp