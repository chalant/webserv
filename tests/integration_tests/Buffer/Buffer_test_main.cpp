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
 *       -The BufferManager is responsible for managing buffers, it can push data to buffers, flush buffers, and destroy buffers.
 *       -The FileBuffer and SocketBuffer classes are used to store data read from files and sockets, respectively, and are controlled by the BufferManager.
 *       -The Logger class pushing messages to the buffer through the buffer manager.
 *       -The EventManager class flushes buffers at the appropriate time through the buffer manager.
 */

#include <cassert>
#include <fcntl.h>
#include <sys/stat.h>

// include the header files for the classes under test
#include "../../../includes/buffer/BufferManager.hpp"
#include "../../../includes/logger/Logger.hpp"
#include "../../../includes/core/EventManager.hpp"
#include "../../../includes/configuration/ConfigurationLoader.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockPollfdManager.hpp"
#include "../../mock_includes/MockSocket.hpp"
#include "../../mock_includes/MockServer.hpp"
#include "../../mock_includes/MockRequestHandler.hpp"
#include "../../mock_includes/MockLoggerConfiguration.hpp"

int main()
{
  // Mock objects
  MockPollfdManager mockPollfdManager;
  MockSocket mockSocket;
  MockServer mockServer;
  MockRequestHandler mockRequestHandler;
  // MockConfigurationBlock mockConfigurationBlock;

  // Instantiate the object under test
  BufferManager bufferManager(mockSocket);
  Logger logger(bufferManager);
  EventManager eventManager(mockPollfdManager, bufferManager, mockServer, mockRequestHandler, logger);
  ConfigurationLoader confLoader(logger);

  const IConfiguration &mockConfigurationBlock = confLoader.loadConfiguration("test_configuration.conf");
  // Open a test file, set flush threshold at 50 and mock add the fd to pollfd.
  int fd = open("temp.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
  MockLoggerConfiguration mockLoggerConfiguration(bufferManager, mockConfigurationBlock, mockPollfdManager);

  mockLoggerConfiguration.setFileDescriptor(fd);
  mockLoggerConfiguration.setBufferSize(50);
  logger.configure(mockLoggerConfiguration);
  pollfd pollfd;
  pollfd.fd = fd;
  pollfd.events = POLLOUT;
  pollfd.revents = POLLOUT;
  mockPollfdManager.addRegularFilePollfd(pollfd);

  // Test case 1: FileBuffer: Pushing data to the buffer by the logger
  //******************************************************************
  // Push data to the buffer
  std::string testString = "This is a test message";
  int returnValue = logger.log(testString);

  // Check the buffer
  std::vector<char> bufferVector = bufferManager.peekBuffer(fd);
  std::string buffer(bufferVector.begin(), bufferVector.end());

  // Verify the data is in the buffer, and the return value is 0
  assert(buffer.find(testString) != buffer.npos);
  // Verify the return value is 0 - no flush is requested
  assert(returnValue == 0);
  // Verify that the file is empty
  assert(lseek(fd, 0, SEEK_END) == 0);

  // Test case 2: FileBuffer: Push more data and verify a flush is requested
  //************************************************************************
  // Push data to the buffer
  std::string testString2 = "This is another test message";
  returnValue = logger.log(testString2);

  // Check the buffer
  bufferVector = bufferManager.peekBuffer(fd);
  buffer = std::string(bufferVector.begin(), bufferVector.end());

  // Verify the data is in the buffer
  assert(buffer.find(testString2) != buffer.npos);
  // Verify the previous data is in the buffer
  assert(buffer.find(testString) != buffer.npos);
  // Verify that a flush is requested
  assert(returnValue == 1);
  // Verify that the file is still empty
  assert(lseek(fd, 0, SEEK_END) == 0);

  // Test case 3: SocketBuffer: Pushing data to the buffer
  //******************************************************
  // Push data to the buffer
  std::string testString3 = "This is a socket msg";
  std::vector<char> testString3Vector(testString3.begin(), testString3.end());
  ssize_t returnVal = bufferManager.pushSocketBuffer(0, testString3Vector);

  // Check the filebuffer buffer
  bufferVector = bufferManager.peekBuffer(fd);
  buffer = std::string(bufferVector.begin(), bufferVector.end());

  // Check the socket buffer
  bufferVector = bufferManager.peekBuffer(0);
  std::string buffer2(bufferVector.begin(), bufferVector.end());

  // Verify the data is in the buffer
  assert(buffer2.find(testString3) != buffer2.npos);
  // Verify the previous data is in the buffer
  assert(buffer.find(testString2) != buffer.npos);
  // Verify the previous data is in the buffer
  assert(buffer.find(testString) != buffer.npos);
  // Verify that the file is still empty
  assert(lseek(fd, 0, SEEK_END) == 0);
  // Verify the return value is the number of bytes pushed
  assert(returnVal != -1 && returnVal == static_cast<ssize_t>(testString3Vector.size()));

  // Test case 4: FileBuffer: Flushing the buffer by the event manager
  //******************************************************************
  // Flush the buffer
  eventManager.handleEvents();

  // Check the buffer
  bufferVector = bufferManager.peekBuffer(fd);
  buffer = std::string(bufferVector.begin(), bufferVector.end());

  // Verify the FileBuffer data is not in the buffer
  assert(buffer.find(testString) == buffer.npos);
  assert(buffer.find(testString2) == buffer.npos);
  // Verify the SocketBuffer data is still in the buffer
  assert(buffer2.find(testString3) != buffer2.npos);
  // Verify that the file has now been written to
  assert(lseek(fd, 0, SEEK_END) != 0);

  // Clean up
  bufferManager.destroyBuffer(fd);
  close(fd);
  unlink("temp.txt");
}