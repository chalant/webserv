#include "../../includes/logger/LoggerConfiguration.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include <unistd.h>

LoggerConfiguration::LoggerConfiguration(IBufferManager &BufferManager, const IConfiguration &configuration, IPollfdManager &pollfdManager)
    :  _accessLogFile(configuration.getBlocks("http")[0]->getBlocks("server")[0]->getString("access_log")), // Currently only supports one access log file
      _bufferManager(BufferManager),
      _pollfdManager(pollfdManager),
      _bufferSize(LOG_BUFFER_SIZE),
      _accessLogFileDescriptor(this->_accessLogFile == "off" ? -2 : open(this->_accessLogFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _accessLogEnabled(this->_accessLogFileDescriptor < 0 ? false : true),
      _logLevelHelper()
{
    // Set the error log file as the first word in the error_log directive
    this->_errorLogFile =  configuration.getString("error_log", 0);
    // Open the error log file if it is not set to "off"
    this->_errorLogFileDescriptor = this->_errorLogFile == "off" ? -2 : open(this->_errorLogFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    // Disable error logging if the error log file is set to "off" or opening the file failed
    this->_errorLogEnabled = this->_errorLogFileDescriptor < 0 ? false : true;

    // Set the log level as the second word in the error_log directive
    try 
    {
        std::string logLevel = configuration.getString("error_log", 1);
        this->_logLevel = this->_logLevelHelper.stringLogLevelMap(logLevel);
    } catch (const std::exception &e) {
        this->_logLevel = DEFAULT_LOG_LEVEL; // If the log level is not set, or is invalid, set it to the default log level
    }

    // Throw an exception if the log file could not be opened
    if (this->_errorLogFileDescriptor == -1 || this->_accessLogFileDescriptor == -1)
        throw LogFileOpenError();

    // Set the buffer size
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
