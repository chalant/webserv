#include "../mock_includes/MockSession.hpp"

/*
 * MockSession class
 *
 * This class is a mock implementation of the ISession interface.
 * It is used for testing purposes.
 *
 */

// Destructor
MockSession::~MockSession()
{
}

void MockSession::touch()
{
}

bool MockSession::hasExpired() const
{
    return false;
}

void MockSession::setData(const std::string &, const std::string &)
{
}

std::string MockSession::getData(const std::string &) const
{
    return "";
}

SessionId_t MockSession::getId() const
{
    return 0;
}

// Path: tests/mock_srcs/MockConnectionManager.cpp
