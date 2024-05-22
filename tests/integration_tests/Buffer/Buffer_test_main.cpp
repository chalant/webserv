/*
 * Integration test: Buffering
 *
 * Components:
 *       -FileBuffer Class (IBuffer)
 *       -SocketBuffer Class (IBuffer)
 *       -BufferManager Class (IBufferManager)
 *       -Logger Class (ILogger)
 *       -EventManager Class
 *
 * Description:
 *       -The BufferManager is responsible for managing buffers, it can push
 * data to buffers, flush buffers, and destroy buffers. -The FileBuffer and
 * SocketBuffer classes are used to store data read from files and sockets,
 * respectively, and are controlled by the BufferManager. -The Logger class
 * pushing messages to the buffer through the buffer manager. -The EventManager
 * class flushes buffers at the appropriate time through the buffer manager.
 */

#include <cassert>
#include <fcntl.h>
#include <sys/stat.h>

// include the header files for the classes under test
#include "../../../includes/buffer/BufferManager.hpp"
#include "../../../includes/configuration/ConfigurationLoader.hpp"
#include "../../../includes/core/EventManager.hpp"
#include "../../../includes/logger/Logger.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockLoggerConfiguration.hpp"
#include "../../mock_includes/MockPollfdManager.hpp"
#include "../../mock_includes/MockRequestHandler.hpp"
#include "../../mock_includes/MockServer.hpp"
#include "../../mock_includes/MockSocket.hpp"

int main()
{
    // Mock objects
    MockPollfdManager mock_pollfd_manager;
    MockSocket mock_socket;
    MockServer mock_server;
    MockRequestHandler mock_request_handler;

    // Instantiate the object under test
    BufferManager buffer_manager(mock_socket);
    Logger logger(buffer_manager);
    EventManager eventManager(mock_pollfd_manager, buffer_manager, mock_server,
                              mock_request_handler, logger);
    ConfigurationLoader conf_loader(logger);

    const IConfiguration &mock_configuration_block =
        conf_loader.loadConfiguration("test_configuration.conf");
    // Open a test file, set flush threshold at 50 and mock add the fd to
    // pollfd.
    int fd = open("temp.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    MockLoggerConfiguration mock_logger_configuration(
        buffer_manager, mock_configuration_block, mock_pollfd_manager);

    mock_logger_configuration.setFileDescriptor(fd);
    mock_logger_configuration.setBufferSize(50);
    logger.configure(mock_logger_configuration);
    pollfd pollfd;
    pollfd.fd = fd;
    pollfd.events = POLLOUT;
    pollfd.revents = POLLOUT;
    mock_pollfd_manager.addRegularFilePollfd(pollfd);

    // Test case 1: FileBuffer: Pushing data to the buffer by the logger
    //******************************************************************
    // Push data to the buffer
    std::string test_string = "This is a test message";
    int return_value = logger.log(test_string);

    // Check the buffer
    std::vector<char> buffer_vector = buffer_manager.peekBuffer(fd);
    std::string buffer(buffer_vector.begin(), buffer_vector.end());

    // Verify the data is in the buffer, and the return value is 0
    assert(buffer.find(test_string) != buffer.npos);
    // Verify the return value is 0 - no flush is requested
    assert(return_value == 0);
    // Verify that the file is empty
    assert(lseek(fd, 0, SEEK_END) == 0);

    // Test case 2: FileBuffer: Push more data and verify a flush is requested
    //************************************************************************
    // Push data to the buffer
    std::string test_string2 = "This is another test message";
    return_value = logger.log(test_string2);

    // Check the buffer
    buffer_vector = buffer_manager.peekBuffer(fd);
    buffer = std::string(buffer_vector.begin(), buffer_vector.end());

    // Verify the data is in the buffer
    assert(buffer.find(test_string2) != buffer.npos);
    // Verify the previous data is in the buffer
    assert(buffer.find(test_string) != buffer.npos);
    // Verify that a flush is requested
    assert(return_value == 1);
    // Verify that the file is still empty
    assert(lseek(fd, 0, SEEK_END) == 0);

    // Test case 3: SocketBuffer: Pushing data to the buffer
    //******************************************************
    // Push data to the buffer
    std::string test_string3 = "This is a socket msg";
    std::vector<char> test_string3Vector(test_string3.begin(), test_string3.end());
    ssize_t returnVal = buffer_manager.pushSocketBuffer(0, test_string3Vector);

    // Check the filebuffer buffer
    buffer_vector = buffer_manager.peekBuffer(fd);
    buffer = std::string(buffer_vector.begin(), buffer_vector.end());

    // Check the socket buffer
    buffer_vector = buffer_manager.peekBuffer(0);
    std::string buffer2(buffer_vector.begin(), buffer_vector.end());

    // Verify the data is in the buffer
    assert(buffer2.find(test_string3) != buffer2.npos);
    // Verify the previous data is in the buffer
    assert(buffer.find(test_string2) != buffer.npos);
    // Verify the previous data is in the buffer
    assert(buffer.find(test_string) != buffer.npos);
    // Verify that the file is still empty
    assert(lseek(fd, 0, SEEK_END) == 0);
    // Verify the return value is the number of bytes pushed
    assert(returnVal != -1 &&
           returnVal == static_cast<ssize_t>(test_string3Vector.size()));

    // Test case 4: FileBuffer: Flushing the buffer by the event manager
    //******************************************************************
    // Flush the buffer
    eventManager.handleEvents();

    // Check the buffer
    buffer_vector = buffer_manager.peekBuffer(fd);
    buffer = std::string(buffer_vector.begin(), buffer_vector.end());

    // Verify the FileBuffer data is not in the buffer
    assert(buffer.find(test_string) == buffer.npos);
    assert(buffer.find(test_string2) == buffer.npos);
    // Verify the SocketBuffer data is still in the buffer
    assert(buffer2.find(test_string3) != buffer2.npos);
    // Verify that the file has now been written to
    assert(lseek(fd, 0, SEEK_END) != 0);

    // Clean up
    buffer_manager.destroyBuffer(fd);
    close(fd);
    unlink("temp.txt");
}
