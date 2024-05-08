#include "../mock_includes/MockLoggerConfiguration.hpp"

/*
 * MockLoggerConfiguration.cpp
 * Mocks the LoggerConfiguration class
 */

MockLoggerConfiguration::MockLoggerConfiguration(IBufferManager &bufferManager, const IConfiguration &configuration, IPollfdManager &pollfdManager)
    : _errorLogFile(""),
      _accessLogFile(""),
      _bufferManager(bufferManager),
      _bufferSize(99999),
      _errorLogFileDescriptor(STDOUT_FILENO),
      _accessLogFileDescriptor(STDOUT_FILENO),
      _errorLogEnabled(true),
      _accessLogEnabled(true)
{
    static_cast<void>(configuration);
    static_cast<void>(pollfdManager);
}
MockLoggerConfiguration::~MockLoggerConfiguration() {}

void MockLoggerConfiguration::setErrorLogEnabled(bool enabled)
{
    this->_errorLogEnabled = enabled;
}

void MockLoggerConfiguration::setAccessLogEnabled(bool enabled)
{
    this->_accessLogEnabled = enabled;
}

int MockLoggerConfiguration::getErrorLogFileDescriptor() const
{
    return this->_errorLogFileDescriptor;
}

int MockLoggerConfiguration::getAccessLogFileDescriptor() const
{
    return this->_accessLogFileDescriptor;
}

IBufferManager &MockLoggerConfiguration::getBufferManager() const
{
    IBufferManager *bufferManager = nullptr;
    return *bufferManager; // Returning a nullptr is not allowed
}

std::string MockLoggerConfiguration::getErrorLogFile() const { return ""; }

std::string MockLoggerConfiguration::getAccessLogFile() const { return ""; }

LogLevel MockLoggerConfiguration::getLogLevel() const { return DEBUG; }

bool MockLoggerConfiguration::getErrorLogEnabled() const
{
    return this->_errorLogEnabled;
}

bool MockLoggerConfiguration::getAccessLogEnabled() const
{
    return this->_accessLogEnabled;
}

void MockLoggerConfiguration::requestFlush(int descriptor) { static_cast<void>(descriptor); }

// Test specific methods
void MockLoggerConfiguration::setBufferSize(size_t size)
{
    this->_bufferSize = size;
    this->_bufferManager.setFlushThreshold(size);
}

void MockLoggerConfiguration::setFileDescriptor(int errorLogFileDescriptor, int accessLogFileDescriptor)
{
    this->_errorLogFileDescriptor = errorLogFileDescriptor;
    this->_accessLogFileDescriptor = accessLogFileDescriptor;
}

// Path: tests/mock_srcs/MockLoggerConfiguration.cpp
