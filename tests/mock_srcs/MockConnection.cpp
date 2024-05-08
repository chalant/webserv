#include "../mock_includes/MockConnection.hpp"

/*
 * MockConnection class
 *
 * This class is a mock implementation of the IConnection interface.
 * It is used for testing purposes.
 *
 */

MockConnection::MockConnection() {}

MockConnection::~MockConnection() {}

// Setters
void MockConnection::setSession(ISession *session) { static_cast<void>(session); }

// Getters
int MockConnection::getSocketDescriptor() const { return 0; }

std::string MockConnection::getIp() const { return std::string(); }

int MockConnection::getPort() const { return 0; }

std::string MockConnection::getRemoteAddress() const { return std::string(); }

int MockConnection::getResponseReadPipefd() const { return 0; }

IRequest &MockConnection::getRequest() const { return *this->_m_request; }

IResponse &MockConnection::getResponse() const { return *this->_m_response; }

ISession &MockConnection::getSession() const { return *this->_m_session; }

void MockConnection::setCgiInfo(int cgiPid, int responseReadPipefd, int requestWritePipefd) {
    static_cast<void>(cgiPid);
    static_cast<void>(responseReadPipefd);
    static_cast<void>(requestWritePipefd);
}

// Connection management
void MockConnection::touch() {}

bool MockConnection::hasExpired() const { return false; }

// Path: tests/mock_srcs/MockConnection.cpp
