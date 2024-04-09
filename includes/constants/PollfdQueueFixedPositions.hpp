#ifndef POLLFDQUEUEFIXEDPOSITIONS_HPP
#define POLLFDQUEUEFIXEDPOSITIONS_HPP

// Enum class for fixed position file descriptors in the pollfd queue

enum PollfdQueueFixedPositions
{
    SERVER_SOCKET_POLL_FD = 0, // The server socket file descriptor position
    ERROR_LOG_POLL_FD = 1,     // The error log file descriptor position
    ACCESS_LOG_POLL_FD = 2,    // The access log file descriptor position
    FIRST_CLIENT_POLL_FD = 3   // The first client file descriptor position
};

#endif // POLLFDQUEUEFIXEDPOSITIONS_HPP
       // Path: includes/constants/PollfdQueueFixedPositions.hpp