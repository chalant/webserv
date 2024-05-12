#include "../../includes/logger/LoggerConfiguration.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include <unistd.h>

LoggerConfiguration::LoggerConfiguration(IBufferManager &BufferManager, const IConfiguration &configuration, IPollfdManager &pollfdManager)
    : _errorLogFile(configuration.getString("error_log")),
      _accessLogFile(configuration.getBlocks("http")[0]->getBlocks("server")[0]->getString("access_log")), // Currently only supports one access log file
      _bufferManager(BufferManager),
      _pollfdManager(pollfdManager),
      _bufferSize(LOG_BUFFER_SIZE),
      _logLevel(LOG_LEVEL),
      _errorLogFileDescriptor(this->_errorLogFile == "off" ? -2 : open(this->_errorLogFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _accessLogFileDescriptor(this->_accessLogFile == "off" ? -2 : open(this->_accessLogFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _errorLogEnabled(this->_errorLogFileDescriptor < 0 ? false : true),
      _accessLogEnabled(this->_accessLogFileDescriptor < 0 ? false : true)
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
    pollfd pollfd;
    pollfd.fd = descriptor;
    pollfd.events = POLLOUT;
    pollfd.revents = 0;
    this->_pollfdManager.addRegularFilePollfd(pollfd);
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
