#include "../../includes/utils/SignalHandler.hpp"
#include <csignal>
#include "../../includes/exception/WebservExceptions.hpp"

bool SignalHandler::m_sigint_received = false;

SignalHandler::SignalHandler()
{
	m_sigint_received = false;
}

SignalHandler::~SignalHandler()
{
}

void SignalHandler::m_sigintHandler(int param)
{
	static_cast<void>(param);
	m_sigint_received = true;
}


void SignalHandler::sigint()
{
	std::signal(SIGINT, m_sigintHandler);
}

void SignalHandler::checkState()
{
	if (m_sigint_received)
		throw SigintException();
}
