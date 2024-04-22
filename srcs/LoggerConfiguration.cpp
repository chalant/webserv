#include "../includes/LoggerConfiguration.hpp"

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
}

LoggerConfiguration::~LoggerConfiguration()
{
    if (this->_errorLogFileDescriptor != -1)
        close(this->_errorLogFileDescriptor);
    if (this->_accessLogFileDescriptor != -1)
        close(this->_accessLogFileDescriptor);
}

void LoggerConfiguration::requestFlush(int descriptor)
{
    this->_pollfdManager.addRegularFilePollfd({descriptor, POLLOUT, 0});
}
