#ifndef MOCKCONNECTION_HPP
#define MOCKCONNECTION_HPP

/*
 * MockConnection class
 *
 * This class is a mock implementation of the IConnection interface.
 * It is used for testing purposes.
 *
 */

#include "../../includes/connection/IConnection.hpp"

class MockConnection : public IConnection
{
private:
    ISession *_m_session;
    IRequest *_m_request;
    IResponse *_m_response;

public:
    MockConnection();
    ~MockConnection();

    // Setters
    void setSession(ISession *session);

    // Getters
    int getSocketDescriptor() const;
    std::string getIp() const;
    int getPort() const;
    std::string getRemoteAddress() const;
    virtual int getCgiInputPipeWriteEnd() const;
    virtual int getCgiOutputPipeReadEnd() const;
    IRequest &getRequest() const;
    IResponse &getResponse() const;
    ISession &getSession() const;
    void setCgiInfo(int cgi_pid, int response_read_pipe_fd,
                    int request_write_pipe_fd);

    // Connection management
    void touch();
    bool hasExpired() const;
};

#endif // MOCKCONNECTION_HPP
// Path: tests/mock_includes/MockConnection.hpp
