/*
 *   Integration test: Session Cookies
 *
 *   Components:
 *       -Session Class (ISession)
 *       -Connection Manager Class (IConnectionManager)
 *       -Request Class (IRequest)
 *       -RequestParser Class (IRequestParser)
 *       -Response Class (IResponse)
 *
 *   Description:
 *       - The Connection Manager assigns sessions to connections
 *       - It read session cookies from the request
 *       - The RequestParser parses cookies into the request
 *       - The connection Manager adds a session cookie to the response
 */

#include <cassert>

// Include the classes to be tested
#include "../../../includes/connection/ConnectionManager.hpp"
#include "../../../includes/request/RequestParser.hpp"
#include "../../../includes/response/Response.hpp"
#include "../../../includes/request/Request.hpp"
#include "../../../includes/constants/HttpHelper.hpp"
#include "../../../includes/configuration/ConfigurationLoader.hpp"

// Include the mock classes
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockFactory.hpp"

// #include "MockConfigurationBlock.hpp"
#include "../../mock_includes/MockConnection.hpp"

// Test helper function
std::vector<char> stringToVector(std::string str)
{
  std::vector<char> vec(str.begin(), str.end());
  return vec;
}

int main()
{
  // Mock objects
  MockLogger logger;
  MockFactory factory;
//   MockConfigurationBlock mockConfigurationBlock;
  MockConnection mockConnection;
  ConfigurationLoader	confLoader(logger);

  // Mock Set the Configuration
//   mockConfigurationBlock.setString("DefaultPort", "80");
//   mockConfigurationBlock.setInt("ClientHeaderBufferSize", 4096);
//   mockConfigurationBlock.setSize_t("ClientMaxBodySize", 10000);
//   mockConfigurationBlock.setSize_t("ClientMaxUriSize", 1000);
//   mockConfigurationBlock.setStringVector("Methods", {"GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH"});

  const IConfiguration&	mockConfigurationBlock = confLoader.loadConfiguration("test_configuration.conf");

  // Instantiate the objects under test
  HttpHelper httpHelper;
  Request request(mockConfigurationBlock, httpHelper);
  Response response(httpHelper);
  ConnectionManager connectionManager(logger, factory);
  RequestParser requestParser(mockConfigurationBlock, logger);


  // Test 1: A Request without a session cookie
  //*******************************************
  std::string requestString = "GET /index.html HTTP/1.1\r\n"
                              "Host: 42.fr\r\n"
                              "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0\r\n"
                              "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
                              "Accept-Language: en-US,en;q=0.5\r\n"
                              "Accept-Encoding: gzip, deflate\r\n"
                              "Connection: keep-alive\r\n"
                              "Referer: http://example.com/\r\n"
                              "Cookie: username=JohnDoe; theme=dark\r\n"
                              "\r\n";
  std::vector<char> rawRequest = stringToVector(requestString);

  // Parse the request
  requestParser.parseRequest(rawRequest, request);

  // Verify that there are no Active Sessions
  assert(connectionManager.getNumberOfSessions() == 0);

  // Assign a session to the connection
  connectionManager.assignSessionToConnection(mockConnection, request, response);

  // Verify that there is now an active session
  assert(connectionManager.getNumberOfSessions() == 1);

  // Verify that a session cookie was added to the response
  std::string sessionCookieId = response.getCookie("session");
  assert(sessionCookieId != "");


  //Test2: Another request without a session cookie
  //***********************************************
  requestString = "GET /index.html HTTP/1.1\r\n"
                  "Host: 42.fr\r\n"
                  "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0\r\n"
                  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
                  "Accept-Language: en-US,en;q=0.5\r\n"
                  "Accept-Encoding: gzip, deflate\r\n"
                  "Connection: keep-alive\r\n"
                  "Referer: http://example.com/\r\n"
                  "Cookie: username=JohnDoe; theme=light\r\n"
                  "\r\n";
  rawRequest = stringToVector(requestString);

  // Parse the request
  requestParser.parseRequest(rawRequest, request);

  // Verify that there is still only one active session
  assert(connectionManager.getNumberOfSessions() == 1);

  // Assign a session to the connection
  connectionManager.assignSessionToConnection(mockConnection, request, response);

  // Verify that there are now two active sessions
  assert(connectionManager.getNumberOfSessions() == 2);

  // Test 3: A request with the session cookie from test 1
  //******************************************************
  requestString = "GET /index.html HTTP/1.1\r\n"
                  "Host: 42.fr\r\n"
                  "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0\r\n"
                  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
                  "Accept-Language: en-US,en;q=0.5\r\n"
                  "Accept-Encoding: gzip, deflate\r\n"
                  "Connection: keep-alive\r\n"
                  "Referer: http://example.com/\r\n"
                  "Cookie: username=JohnDoe; theme=light; session=" + sessionCookieId + "\r\n"
                  "\r\n";
  rawRequest = stringToVector(requestString);

  // Parse the request
  requestParser.parseRequest(rawRequest, request);

  // Verify that there is a session cookie in the request
  std::string sessionCookieFromRequest = request.getCookie("session");
  assert(sessionCookieFromRequest != "");

  // Verify that the session ID in the request matches the session ID in the response
  assert(sessionCookieFromRequest == sessionCookieId);

  // Verify that there are still only two active sessions
  assert(connectionManager.getNumberOfSessions() == 2);

  // Assign a session to the connection
  connectionManager.assignSessionToConnection(mockConnection, request, response);

  // Verify that the session ID was re-used
  assert(connectionManager.getNumberOfSessions() == 2);

  return (0);
}