#include "logger/LoggerConfiguration.hpp"

LoggerConfiguration::LoggerConfiguration(IBufferManager &BufferManager, IConfiguration &configuration, IPollfdManager &pollfdManager)
    : _errorLogFile(configuration.getString("errorLog")),
      _accessLogFile(configuration.getString("accessLog")),
      _bufferManager(BufferManager),
      _pollfdManager(pollfdManager),
      _logLevel(static_cast<LogLevel>(configuration.getInt("LogLevel"))),
      _bufferSize(configuration.getInt("BufferSize")),
      _errorLogFileDescriptor(open(this->_errorLogFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _accessLogFileDescriptor(open(this->_accessLogFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _errorLogEnabled(configuration.getBool("errorLogEnabled")),
      _accessLogEnabled(configuration.getBool("accessLogEnabled"))
{
    if (this->_errorLogFileDescriptor == -1 || this->_accessLogFileDescriptor == -1)
        throw LogFileOpenError();
    this->_bufferManager.setFlushThreshold(this->_bufferSize);
}

LoggerConfiguration::~LoggerConfiguration()
{
    this->_bufferManager.flushBuffer(this->_errorLogFileDescriptor);
    this->_bufferManager.flushBuffer(this->_accessLogFileDescriptor);
    if (this->_errorLogFileDescriptor != -1)
        close(this->_errorLogFileDescriptor);
    if (this->_accessLogFileDescriptor != -1)
        close(this->_accessLogFileDescriptor);
}

void LoggerConfiguration::setErrorLogEnabled(bool enabled)
{
    this->_errorLogEnabled = enabled;
}

void LoggerConfiguration::setAccessLogEnabled(bool enabled)
{
    this->_accessLogEnabled = enabled;
}

void LoggerConfiguration::requestFlush(int descriptor)
{
    this->_pollfdManager.addRegularFilePollfd({descriptor, POLLOUT, 0});
}

int LoggerConfiguration::getErrorLogFileDescriptor() const
{
    return this->_errorLogFileDescriptor;
}

int LoggerConfiguration::getAccessLogFileDescriptor() const
{
    return this->_accessLogFileDescriptor;
}

IBufferManager &LoggerConfiguration::getBufferManager() const
{
    return this->_bufferManager;
}

std::string LoggerConfiguration::getErrorLogFile() const
{
    return this->_errorLogFile;
}

std::string LoggerConfiguration::getAccessLogFile() const
{
    return this->_accessLogFile;
}

LogLevel LoggerConfiguration::getLogLevel() const
{
    return this->_logLevel;
}

bool LoggerConfiguration::getErrorLogEnabled() const
{
    return this->_errorLogEnabled;
}

bool LoggerConfiguration::getAccessLogEnabled() const
{
    return this->_accessLogEnabled;
}

// Path: srcs/Logger.cpp
