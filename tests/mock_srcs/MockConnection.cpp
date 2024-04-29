#include "MockConnection.hpp"

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
void MockConnection::setReadPipeDescriptor(int pipe) { static_cast<void>(pipe); }

void MockConnection::setSession(ISession *session) { static_cast<void>(session); }

// Getters
int MockConnection::getSocketDescriptor() const { return 0; }

const std::string MockConnection::getIp() const { return std::string(); }

int MockConnection::getPort() const { return 0; }

const std::string MockConnection::getRemoteAddress() const { return std::string(); }

int MockConnection::getReadPipeDescriptor() const { return 0; }

IRequest &MockConnection::getRequest() const { return *this->_m_request; }

IResponse &MockConnection::getResponse() const { return *this->_m_response; }

ISession &MockConnection::getSession() const { return *this->_m_session; }

// Path: tests/mock_srcs/MockConnection.cpp
