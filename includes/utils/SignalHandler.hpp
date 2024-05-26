#ifndef SIGNALHANDLER_HPP
#define SIGNALHANDLER_HPP

class SignalHandler
{
	private:
		static void m_sigintHandler(int param);
		static bool m_sigint_received;
	public:
		SignalHandler();
		~SignalHandler();

		static void	sigint();
		static void checkState();
};

#endif
