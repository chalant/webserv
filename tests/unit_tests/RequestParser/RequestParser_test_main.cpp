#include <cassert>

// include the header file for the class under test
#include "../../../includes/constants/HttpStatusCodeHelper.hpp"
#include "../../../includes/exception/WebservExceptions.hpp"
#include "../../../includes/request/RequestParser.hpp"

#include "../../../includes/configuration/ConfigurationLoader.hpp"

// include the header files for the mock classes
#include "../../mock_includes/MockLogger.hpp"
#include "../../mock_includes/MockRequest.hpp"

// Method to initialize a std::vector<char> from a string
std::vector<char> stringToVector(const std::string &str);

int main()
{
    // Mock objects
    // MockConfigurationBlock mock_configuration_block;
    MockLogger mock_logger;
    ConfigurationLoader conf_loader(mock_logger);

    // mock_configuration_block.setInt("client_header_buffer_size", 6000);

    const IConfiguration &mock_configuration_block =
        conf_loader.loadConfiguration("test_configuration.conf");
    // Request Parser instance
    RequestParser request_parser(mock_configuration_block, mock_logger);

    // Recipient of the Request Parser's output
    MockRequest mock_request;

    // test case 1: A Valid IRequest
    //******************************
    // GET /index.html HTTP/1.1
    // Host: 42.fr
    // content-length: 0
    std::vector<char> raw_request =
        stringToVector("GET /index.html HTTP/1.1\r\nHost: "
                       "42.fr\r\ncontent-length: 0\r\n\r\n");

    // Parse the raw request
    request_parser.parseRequest(raw_request, mock_request);
    // Verify the parsed Request object
    // If the request is invalid, an exception will be thrown
    // otherwise the test will pass
    assert(mock_request.getTestMethod() == "GET");
    assert(mock_request.getUri() == "/index.html");
    assert(mock_request.getTestHttpVersion() == "HTTP/1.1");
    std::map<std::string, std::string> expected_headers;
    expected_headers.insert(std::make_pair("host", "42.fr"));
    expected_headers.insert(std::make_pair("content-length", "0"));
    assert(mock_request.getHeadersStringMap() == expected_headers);
    // Clear the contents of the Request object for the next test
    mock_request.clear();

    // test case 2: A multipart/form-data
    //***********************************
    // POST /upload HTTP/1.1
    // Host: 42.fr
    // Content-Length: 326
    // Content-Type: multipart/form-data; boundary=MY_BOUNDARY
    //
    // --MY_BOUNDARY
    // Content-Disposition: form-data; name="field1"
    //
    // value1
    // --MY_BOUNDARY
    // Content-Disposition: form-data; name="file_1"; filename="file1.txt"
    // Content-Type: text/plain
    //
    // Contents of file 1 here
    // --MY_BOUNDARY
    // Content-Disposition: form-data; name="file_2"; filename="file2.txt"
    // Content-Type: text/plain
    //
    // Contents of file 2 here
    // --MY_BOUNDARY--
    raw_request = stringToVector(
        "POST /upload HTTP/1.1\r\nHost: 42.fr\r\nContent-Length: "
        "363\r\nContent-Type: "
        "multipart/form-data; boundary=MY_BOUNDARY\r\n\r\n--MY_BOUNDARY\r\n"
        "Content-Disposition: form-data; name=\"field1\"\r\n\r\nvalue1\r\n"
        "--MY_BOUNDARY\r\nContent-Disposition: form-data; name=\"file_1\"; "
        "filename=\"file1.txt\"\r\nContent-Type: text/plain\r\n\r\nContents of "
        "file 1 here\r\n--MY_BOUNDARY\r\nContent-Disposition: form-data; "
        "name=\"file_2\"; filename=\"file2.txt\"\r\nContent-Type: "
        "text/plain\r\n\r\nContents of "
        "file 2 here\r\n--MY_BOUNDARY--\r\n");

    // Parse the raw request
    request_parser.parseRequest(raw_request, mock_request);

    // Verify that the body_parameters vector contains the correct values
    std::vector<BodyParameter> expected_body_parameters;
    BodyParameter body_parameter_1;
    body_parameter_1.filename = "";
    body_parameter_1.content_type = "";
    body_parameter_1.data.push_back('v');
    body_parameter_1.data.push_back('a');
    body_parameter_1.data.push_back('l');
    body_parameter_1.data.push_back('u');
    body_parameter_1.data.push_back('e');
    body_parameter_1.data.push_back('1');
    body_parameter_1.disposition_type = "form-data";
    body_parameter_1.field_name = "field1";
    expected_body_parameters.push_back(body_parameter_1);
    BodyParameter body_parameter_2;
    body_parameter_2.filename = "file1.txt";
    body_parameter_2.content_type = "text/plain";
    body_parameter_2.data.push_back('C');
    body_parameter_2.data.push_back('o');
    body_parameter_2.data.push_back('n');
    body_parameter_2.data.push_back('t');
    body_parameter_2.data.push_back('e');
    body_parameter_2.data.push_back('n');
    body_parameter_2.data.push_back('t');
    body_parameter_2.data.push_back('s');
    body_parameter_2.data.push_back(' ');
    body_parameter_2.data.push_back('o');
    body_parameter_2.data.push_back('f');
    body_parameter_2.data.push_back(' ');
    body_parameter_2.data.push_back('f');
    body_parameter_2.data.push_back('i');
    body_parameter_2.data.push_back('l');
    body_parameter_2.data.push_back('e');
    body_parameter_2.data.push_back(' ');
    body_parameter_2.data.push_back('1');
    body_parameter_2.data.push_back(' ');
    body_parameter_2.data.push_back('h');
    body_parameter_2.data.push_back('e');
    body_parameter_2.data.push_back('r');
    body_parameter_2.data.push_back('e');
    body_parameter_2.disposition_type = "form-data";
    body_parameter_2.field_name = "file_1";
    expected_body_parameters.push_back(body_parameter_2);
    BodyParameter body_parameter_3;
    body_parameter_3.filename = "file2.txt";
    body_parameter_3.content_type = "text/plain";
    body_parameter_3.data.push_back('C');
    body_parameter_3.data.push_back('o');
    body_parameter_3.data.push_back('n');
    body_parameter_3.data.push_back('t');
    body_parameter_3.data.push_back('e');
    body_parameter_3.data.push_back('n');
    body_parameter_3.data.push_back('t');
    body_parameter_3.data.push_back('s');
    body_parameter_3.data.push_back(' ');
    body_parameter_3.data.push_back('o');
    body_parameter_3.data.push_back('f');
    body_parameter_3.data.push_back(' ');
    body_parameter_3.data.push_back('f');
    body_parameter_3.data.push_back('i');
    body_parameter_3.data.push_back('l');
    body_parameter_3.data.push_back('e');
    body_parameter_3.data.push_back(' ');
    body_parameter_3.data.push_back('2');
    body_parameter_3.data.push_back(' ');
    body_parameter_3.data.push_back('h');
    body_parameter_3.data.push_back('e');
    body_parameter_3.data.push_back('r');
    body_parameter_3.data.push_back('e');
    body_parameter_3.disposition_type = "form-data";
    body_parameter_3.field_name = "file_2";
    expected_body_parameters.push_back(body_parameter_3);

    std::vector<BodyParameter> body_parameters;
    body_parameters = mock_request.getBodyParameters();

    // Assert that the body_parameters vector contains the expected values
    assert(body_parameters.size() == 3);
    assert(body_parameters[ 0 ].filename ==
           expected_body_parameters[ 0 ].filename);
    assert(body_parameters[ 0 ].content_type ==
           expected_body_parameters[ 0 ].content_type);
    assert(body_parameters[ 0 ].data == expected_body_parameters[ 0 ].data);
    assert(body_parameters[ 0 ].disposition_type ==
           expected_body_parameters[ 0 ].disposition_type);
    assert(body_parameters[ 0 ].field_name ==
           expected_body_parameters[ 0 ].field_name);
    assert(body_parameters[ 1 ].filename ==
           expected_body_parameters[ 1 ].filename);
    assert(body_parameters[ 1 ].content_type ==
           expected_body_parameters[ 1 ].content_type);
    assert(body_parameters[ 1 ].data == expected_body_parameters[ 1 ].data);
    assert(body_parameters[ 1 ].disposition_type ==
           expected_body_parameters[ 1 ].disposition_type);
    assert(body_parameters[ 1 ].field_name ==
           expected_body_parameters[ 1 ].field_name);
    assert(body_parameters[ 2 ].filename ==
           expected_body_parameters[ 2 ].filename);
    assert(body_parameters[ 2 ].content_type ==
           expected_body_parameters[ 2 ].content_type);
    assert(body_parameters[ 2 ].data == expected_body_parameters[ 2 ].data);
    assert(body_parameters[ 2 ].disposition_type ==
           expected_body_parameters[ 2 ].disposition_type);
    assert(body_parameters[ 2 ].field_name ==
           expected_body_parameters[ 2 ].field_name);

    // Clear the contents of the Request object for the next test
    mock_request.clear();

    return 0;
}

// Method to initialize a std::vector<char> from a string
std::vector<char> stringToVector(const std::string &str)
{
    std::vector<char> vec;
    for (size_t i = 0; i < str.size(); i++)
    {
        vec.push_back(str[ i ]);
    }
    return vec;
}
