#ifndef MOCKLOGGERCONFIGURATION_HPP
#define MOCKLOGGERCONFIGURATION_HPP

/*
 * MockLoggerConfiguration.hpp
 * Mocks the LoggerConfiguration class
 */

#include "../../includes/buffer/IBufferManager.hpp"
#include "../../includes/configuration/IConfiguration.hpp"
#include "../../includes/logger/ILoggerConfiguration.hpp"
#include "../../includes/pollfd/IPollfdManager.hpp"
#include <unistd.h>

class MockLoggerConfiguration : public ILoggerConfiguration
{
private:
    std::string m_error_log_file;
    std::string m_access_log_file;
    IBufferManager &m_buffer_manager;
    size_t m_buffer_size;
    int m_error_log_file_descriptor;
    int m_access_log_file_descriptor;
    bool m_error_log_enabled;
    bool m_access_log_enabled;

public:
    MockLoggerConfiguration(IBufferManager &buffer_manager,
                            const IConfiguration &configuration,
                            IPollfdManager &pollfd_manager);

    virtual ~MockLoggerConfiguration();
    virtual void setErrorLogEnabled(bool);
    virtual void setAccessLogEnabled(bool);
    virtual int getErrorLogFileDescriptor() const;
    virtual int getAccessLogFileDescriptor() const;
    virtual IBufferManager &getBufferManager() const;
    virtual std::string getErrorLogFile() const;
    virtual std::string getAccessLogFile() const;
    virtual LogLevel getLogLevel() const;
    virtual bool getErrorLogEnabled() const;
    virtual bool getAccessLogEnabled() const;
    virtual void requestFlush(int descriptor);

    // Testing purposes
    virtual void setBufferSize(size_t buffer_size);
    virtual void setFileDescriptor(int error_log_file_descriptor,
                                   int access_log_file_descriptor = STDOUT_FILENO);
};

#endif // MOCKLOGGERCONFIGURATION_HPP
       // Path: tests/mock_includes/MockLoggerConfiguration.hpp
