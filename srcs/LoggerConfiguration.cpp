#include "../includes/LoggerConfiguration.hpp"

LoggerConfiguration::LoggerConfiguration(LoggerType type, IBufferManager *BufferManager, IConfiguration *configuration, IPollfdManager *pollfdManager)
    : _type(type),
      _logFile(type == ERRORLOGGER ? configuration->getString("error_log") : configuration->getString("access_log")),
      _bufferManager(BufferManager),
      _logLevel(static_cast<LogLevel>(configuration->getInt("LogLevel"))),
      _bufferSize(configuration->getInt("BufferSize")),
      _logFileDescriptor(open(this->_logFile.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      _enabled((type == ERRORLOGGER) ? configuration->getBool("ErrorLogEnabled") : configuration->getBool("AccessLogEnabled"))
{
    if (this->_logFileDescriptor == -1)
        throw LogFileOpenError();
    pollfdManager->addFileDescriptorPollfd({this->_logFileDescriptor, POLLOUT, 0});
}

LoggerConfiguration::~LoggerConfiguration()
{
    if (this->_logFileDescriptor != -1)
        close(this->_logFileDescriptor);
}

int LoggerConfiguration::getLogFileDescriptor() const
{
    return this->_logFileDescriptor;
}

IBufferManager *LoggerConfiguration::getBufferManager() const
{
    return this->_bufferManager;
}

LoggerType LoggerConfiguration::getLoggerType() const
{
    return this->_type;
}

std::string LoggerConfiguration::getLogFile() const
{
    return this->_logFile;
}

LogLevel LoggerConfiguration::getLogLevel() const
{
    return this->_logLevel;
}

bool LoggerConfiguration::getEnabled() const
{
    return this->_enabled;
}