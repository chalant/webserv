#include "../mock_includes/MockLoggerConfiguration.hpp"

/*
 * MockLoggerConfiguration.cpp
 * Mocks the LoggerConfiguration class
 */

MockLoggerConfiguration::MockLoggerConfiguration(
    IBufferManager &buffer_manager, const IConfiguration &configuration,
    IPollfdManager &pollfd_manager)
    : m_error_log_file(""), m_access_log_file(""),
      m_buffer_manager(buffer_manager), m_buffer_size(99999),
      m_error_log_file_descriptor(STDOUT_FILENO),
      m_access_log_file_descriptor(STDOUT_FILENO), m_error_log_enabled(true),
      m_access_log_enabled(true)
{
    static_cast<void>(configuration);
    static_cast<void>(pollfd_manager);
}
MockLoggerConfiguration::~MockLoggerConfiguration() {}

void MockLoggerConfiguration::setErrorLogEnabled(bool enabled)
{
    m_error_log_enabled = enabled;
}

void MockLoggerConfiguration::setAccessLogEnabled(bool enabled)
{
    m_access_log_enabled = enabled;
}

int MockLoggerConfiguration::getErrorLogFileDescriptor() const
{
    return m_error_log_file_descriptor;
}

int MockLoggerConfiguration::getAccessLogFileDescriptor() const
{
    return m_access_log_file_descriptor;
}

IBufferManager &MockLoggerConfiguration::getBufferManager() const
{
    IBufferManager *buffer_manager = NULL;
    return *buffer_manager; // Returning a NULL is not allowed
}

std::string MockLoggerConfiguration::getErrorLogFile() const { return ""; }

std::string MockLoggerConfiguration::getAccessLogFile() const { return ""; }

LogLevel MockLoggerConfiguration::getLogLevel() const { return DEBUG; }

bool MockLoggerConfiguration::getErrorLogEnabled() const
{
    return m_error_log_enabled;
}

bool MockLoggerConfiguration::getAccessLogEnabled() const
{
    return m_access_log_enabled;
}

void MockLoggerConfiguration::requestFlush(int descriptor)
{
    static_cast<void>(descriptor);
}

// Test specific methods
void MockLoggerConfiguration::setBufferSize(size_t size)
{
    m_buffer_size = size;
    m_buffer_manager.setFlushThreshold(size);
}

void MockLoggerConfiguration::setFileDescriptor(int error_log_file_descriptor,
                                                int access_log_file_descriptor)
{
    m_error_log_file_descriptor = error_log_file_descriptor;
    m_access_log_file_descriptor = access_log_file_descriptor;
}

// Path: tests/mock_srcs/MockLoggerConfiguration.cpp
