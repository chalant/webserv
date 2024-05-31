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
#include "../../../includes/configuration/ConfigurationLoader.hpp"
#include "../../../includes/connection/ConnectionManager.hpp"
#include "../../../includes/constants/HttpHelper.hpp"
#include "../../../includes/request/Request.hpp"
#include "../../../includes/request/RequestParser.hpp"
#include "../../../includes/response/Response.hpp"

// Include the mock classes
#include "../../mock_includes/MockFactory.hpp"
#include "../../mock_includes/MockLogger.hpp"

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
    //   MockConfigurationBlock mock_configuration_block;
    MockConnection mock_connection;
    ConfigurationLoader conf_loader(logger);

    // Mock Set the Configuration
    //   mock_configuration_block.setString("DefaultPort", "80");
    //   mock_configuration_block.setInt("client_header_buffer_size", 4096);
    //   mock_configuration_block.setSize_t("ClientMaxBodySize", 10000);
    //   mock_configuration_block.setSize_t("client_max_uri_size", 1000);
    //   mock_configuration_block.setStringVector("Methods", {"GET", "POST",
    //   "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH"});

    const IConfiguration &mock_configuration_block =
        conf_loader.loadConfiguration("test_configuration.conf");

    // Instantiate the objects under test
    HttpHelper http_helper;
    Request request(mock_configuration_block, http_helper);
    Response response(http_helper);
    ConnectionManager connection_manager(logger, factory);
    RequestParser request_parser(mock_configuration_block, logger);

    // Test 1: A Request without a session cookie
    //*******************************************
    std::string request_string =
        "GET /index.html HTTP/1.1\r\n"
        "Host: 42.fr\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) "
        "Gecko/20100101 Firefox/98.0\r\n"
        "Accept: "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
        "webp,*/"
        "*;q=0.8\r\n"
        "Accept-Language: en-US,en;q=0.5\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "Connection: keep-alive\r\n"
        "Referer: http://example.com/\r\n"
        "Cookie: username=JohnDoe; theme=dark\r\n"
        "\r\n";
    std::vector<char> raw_request = stringToVector(request_string);

    // Parse the request
    request_parser.parseRequest(raw_request, request);

    // Verify that there are no Active Sessions
    assert(connection_manager.getNumberOfSessions() == 0);

    // Assign a session to the connection
    connection_manager.assignSessionToConnection(mock_connection, request,
                                                 response);

    // Verify that there is now an active session
    assert(connection_manager.getNumberOfSessions() == 1);

    // Verify that a session cookie was added to the response
    std::string session_cookie_id = response.getCookie("session");
    assert(session_cookie_id != "");

    // Test2: Another request without a session cookie
    //***********************************************
    request_string = "GET /index.html HTTP/1.1\r\n"
                     "Host: 42.fr\r\n"
                     "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; "
                     "rv:98.0) Gecko/20100101 Firefox/98.0\r\n"
                     "Accept: "
                     "text/html,application/xhtml+xml,application/"
                     "xml;q=0.9,image/webp,*/*;q=0.8\r\n"
                     "Accept-Language: en-US,en;q=0.5\r\n"
                     "Accept-Encoding: gzip, deflate\r\n"
                     "Connection: keep-alive\r\n"
                     "Referer: http://example.com/\r\n"
                     "Cookie: username=JohnDoe; theme=light\r\n"
                     "\r\n";
    raw_request = stringToVector(request_string);

    // Parse the request
    request_parser.parseRequest(raw_request, request);

    // Verify that there is still only one active session
    assert(connection_manager.getNumberOfSessions() == 1);

    // Assign a session to the connection
    connection_manager.assignSessionToConnection(mock_connection, request,
                                                 response);

    // Verify that there are now two active sessions
    assert(connection_manager.getNumberOfSessions() == 2);

    // Test 3: A request with the session cookie from test 1
    //******************************************************
    request_string = "GET /index.html HTTP/1.1\r\n"
                     "Host: 42.fr\r\n"
                     "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; "
                     "rv:98.0) Gecko/20100101 Firefox/98.0\r\n"
                     "Accept: "
                     "text/html,application/xhtml+xml,application/"
                     "xml;q=0.9,image/webp,*/*;q=0.8\r\n"
                     "Accept-Language: en-US,en;q=0.5\r\n"
                     "Accept-Encoding: gzip, deflate\r\n"
                     "Connection: keep-alive\r\n"
                     "Referer: http://example.com/\r\n"
                     "Cookie: username=JohnDoe; theme=light; session=" +
                     session_cookie_id +
                     "\r\n"
                     "\r\n";
    raw_request = stringToVector(request_string);

    // Parse the request
    request_parser.parseRequest(raw_request, request);

    // Verify that there is a session cookie in the request
    std::string session_cookie_from_request = request.getCookie("session");
    assert(session_cookie_from_request != "");

    // Verify that the session ID in the request matches the session ID in the
    // response
    assert(session_cookie_from_request == session_cookie_id);

    // Verify that there are still only two active sessions
    assert(connection_manager.getNumberOfSessions() == 2);

    // Assign a session to the connection
    connection_manager.assignSessionToConnection(mock_connection, request,
                                                 response);

    // Verify that the session ID was re-used
    assert(connection_manager.getNumberOfSessions() == 2);

    return (0);
}
