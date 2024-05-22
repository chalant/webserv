#include "../../includes/logger/LoggerConfiguration.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include <unistd.h>

LoggerConfiguration::LoggerConfiguration(IBufferManager &BufferManager,
                                         IConfiguration &configuration,
                                         IPollfdManager &pollfdManager)
    : m_access_log_file(configuration.getBlocks("http")[ 0 ]
                         ->getBlocks("server")[ 0 ]
                         ->getString("access_log")), // Currently only supports
                                                     // one access log file
      m_buffer_manager(BufferManager), m_pollfd_manager(pollfdManager),
      m_buffer_size(LOG_BUFFER_SIZE),
      m_access_log_file_descriptor(
          this->m_access_log_file == "off"
              ? -2
              : open(this->m_access_log_file.c_str(),
                     O_WRONLY | O_CREAT | O_APPEND,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)),
      m_access_log_enabled(this->m_access_log_file_descriptor < 0 ? false : true),
      m_log_level_helper()
{
    // Set the error log file as the first word in the error_log directive
    this->m_error_log_file = configuration.getString("error_log", 0);
    // Open the error log file if it is not set to "off"
    this->m_error_log_file_descriptor =
        this->m_error_log_file == "off"
            ? -2
            : open(this->m_error_log_file.c_str(), O_WRONLY | O_CREAT | O_APPEND,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    // Disable error logging if the error log file is set to "off" or opening
    // the file failed
    this->m_error_log_enabled = this->m_error_log_file_descriptor < 0 ? false : true;

    // Set the log level as the second word in the error_log directive
    try
    {
        std::string logLevel = configuration.getString("error_log", 1);
        this->m_log_level = this->m_log_level_helper.stringLogLevelMap(logLevel);
    }
    catch (const std::exception &e)
    {
        this->m_log_level =
            DEFAULT_LOG_LEVEL; // If the log level is not set, or is
                               // invalid, set it to the default log level
    }

    // Throw an exception if the log file could not be opened
    if (this->m_error_log_file_descriptor == -1 ||
        this->m_access_log_file_descriptor == -1)
        throw LogFileOpenError();

    // Set the buffer size
    this->m_buffer_manager.setFlushThreshold(this->m_buffer_size);
}

LoggerConfiguration::~LoggerConfiguration()
{
    this->m_buffer_manager.flushBuffer(this->m_error_log_file_descriptor);
    this->m_buffer_manager.flushBuffer(this->m_access_log_file_descriptor);
    if (this->m_error_log_file_descriptor != -1)
        close(this->m_error_log_file_descriptor);
    if (this->m_access_log_file_descriptor != -1)
        close(this->m_access_log_file_descriptor);
}

void LoggerConfiguration::setErrorLogEnabled(bool enabled)
{
    this->m_error_log_enabled = enabled;
}

void LoggerConfiguration::setAccessLogEnabled(bool enabled)
{
    this->m_access_log_enabled = enabled;
}

void LoggerConfiguration::requestFlush(int descriptor)
{
    pollfd pollfd;
    pollfd.fd = descriptor;
    pollfd.events = POLLOUT;
    pollfd.revents = 0;
    this->m_pollfd_manager.addRegularFilePollfd(pollfd);
}

int LoggerConfiguration::getErrorLogFileDescriptor() const
{
    return this->m_error_log_file_descriptor;
}

int LoggerConfiguration::getAccessLogFileDescriptor() const
{
    return this->m_access_log_file_descriptor;
}

IBufferManager &LoggerConfiguration::getBufferManager() const
{
    return this->m_buffer_manager;
}

std::string LoggerConfiguration::getErrorLogFile() const
{
    return this->m_error_log_file;
}

std::string LoggerConfiguration::getAccessLogFile() const
{
    return this->m_access_log_file;
}

LogLevel LoggerConfiguration::getLogLevel() const { return this->m_log_level; }

bool LoggerConfiguration::getErrorLogEnabled() const
{
    return this->m_error_log_enabled;
}

bool LoggerConfiguration::getAccessLogEnabled() const
{
    return this->m_access_log_enabled;
}

// Path: srcs/Logger.cpp
