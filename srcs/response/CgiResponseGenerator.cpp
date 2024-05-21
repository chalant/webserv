#include "../../includes/response/CgiResponseGenerator.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cerrno>
#include <cstdlib>

#define NO_THROW 0x1 // Do not throw an exception
#define KEEP_RESPONSE_READ_PIPE                                                \
    0x2 // Do no close the read end of the response pipe
#define KEEP_BODY_WRITE_PIPE 0x4 // Do not close the write end of the body pipe

CgiResponseGenerator::CgiResponseGenerator(ILogger &logger)
    : _logger(logger), _httpStatusCodeHelper(HttpStatusCodeHelper())
{
}

CgiResponseGenerator::~CgiResponseGenerator() {}

// calls execve to execute the CGI script
// returns the cgi process Info (pid, read end of the response pipe, write end
// of the body pipe) Throws an exception if an error occurs
Triplet_t CgiResponseGenerator::generateResponse(
    const IRoute &route, const IRequest &request, IResponse &response,
    const IConfiguration &configuration, const std::string &scriptName)
{
    // void the unused IResponse &response
    (void)response;
    
    // Set cgi arguments
    std::vector<char *> cgiArgs;
    this->_setCgiArguments(scriptName, route, configuration, cgiArgs);

    // Set cgi environment variables
    std::vector<char *> cgiEnv;
    this->_setCgiEnvironment(scriptName, route, request, cgiEnv);

    // Create a pipe to send the response from the CGI script to the server
    int responsePipeFd[ 2 ];
    if (pipe(responsePipeFd) == -1)
        // pipe failed
        this->_cleanUp(cgiArgs.data(),
                       cgiEnv.data()); // Free memory and throw exception 500

    // Create a pipe to send the request body from the server to the CGI script
    int bodyPipeFd[ 2 ];
    if (pipe(bodyPipeFd) == -1)
        // pipe failed
        this->_cleanUp(cgiArgs.data(), cgiEnv.data(),
                       responsePipeFd); // Free memory and throw exception 500

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1)
        // fork failed
        this->_cleanUp(cgiArgs.data(), cgiEnv.data(), responsePipeFd,
                       bodyPipeFd); // Free memory and throw exception 500

    else if (pid == 0) // child process
    {
        this->_logger.log(
            DEBUG, "Forked a child process to execute the CGI script PID: " +
                       Converter::toString(getpid()) +
                       " Parent PID: " + Converter::toString(getppid()));
        // stdin should read from body pipe
        close(bodyPipeFd[ 1 ]);              // close write end
        dup2(bodyPipeFd[ 0 ], STDIN_FILENO); // redirect stdin to pipe
        close(bodyPipeFd[ 0 ]);              // close read end

        // stdout should write to response pipe
        close(responsePipeFd[ 0 ]);               // close read end
        dup2(responsePipeFd[ 1 ], STDOUT_FILENO); // redirect stdout to pipe
        close(responsePipeFd[ 1 ]);               // close write end

        // Call execve
        execve(cgiArgs[ 0 ], cgiArgs.data(), cgiEnv.data());
        this->_logger.log(DEBUG,
                          "Execve failed: " + std::string(strerror(errno)));
        // If execve returns, an error occurred; free memory and exit
        this->_cleanUp(cgiArgs.data(), cgiEnv.data(), responsePipeFd,
                       bodyPipeFd, NO_THROW);
        exit(EXIT_FAILURE);
    }

    else // parent process
    {
        // Log the new CGI process ID
        this->_logger.log(DEBUG,
                          "New CGI process ID: " + Converter::toString(pid));

        // Set the read end of the response pipe to non-blocking
        fcntl(responsePipeFd[ 0 ], F_SETFL, O_NONBLOCK);

        // Set the write end of the body pipe to non-blocking
        fcntl(bodyPipeFd[ 1 ], F_SETFL, O_NONBLOCK);

        // Free memory and keep the write end of the body pipe open and the read
        // end of the response pipe open
        this->_cleanUp(
            cgiArgs.data(), cgiEnv.data(), responsePipeFd, bodyPipeFd,
            NO_THROW | KEEP_RESPONSE_READ_PIPE | KEEP_BODY_WRITE_PIPE);

        // Log the Cgi info
        this->_logger.log(
            VERBOSE, "Returning CGI info tuple; PID: " +
                         Converter::toString(pid) + "Response Pipe read end: " +
                         Converter::toString(responsePipeFd[ 0 ]) +
                         "Request Pipe write end: " +
                         Converter::toString(responsePipeFd[ 1 ]));

        // Return the read end of the pipe to read the response later without
        // blocking
        return std::make_pair(
            pid, std::make_pair(responsePipeFd[ 0 ], bodyPipeFd[ 1 ]));
    }

    return std::make_pair(-1, std::make_pair(-1, -1)); // unreachable code
}

void CgiResponseGenerator::_setCgiArguments(const std::string &scriptName,
                                            const IRoute &route,
                                            const IConfiguration &configuration,
                                            std::vector<char *> &cgiArgs)
{
    cgiArgs.push_back(this->_getCgiInterpreterPath(
        scriptName, configuration)); // Interpreter absolute path
    cgiArgs.push_back(this->_getScriptPath(
        scriptName, route)); // script path: location block root path +
                             // URI(excl. query string)
    cgiArgs.push_back(NULL);

    // Check for strdup failures
    for (size_t i = 0; i < cgiArgs.size() - 1; ++i)
        if (cgiArgs[ i ] == NULL)
            this->_cleanUp(
                cgiArgs.data()); // Free memory and throw exception 500

    this->_logger.log(DEBUG, "CGI interpreter: " + std::string(cgiArgs[ 0 ]));
    this->_logger.log(DEBUG, "CGI script: " + std::string(cgiArgs[ 1 ]));
}

void CgiResponseGenerator::_setCgiEnvironment(const std::string &scriptName,
                                              const IRoute &route,
                                              const IRequest &request,
                                              std::vector<char *> &cgiEnv)
{
    cgiEnv.push_back(
        strdup(("REQUEST_METHOD=" + request.getMethodString()).c_str()));
    cgiEnv.push_back(
        strdup(("QUERY_STRING=" + request.getQueryString()).c_str()));
    cgiEnv.push_back(
        strdup(("CONTENT_LENGTH=" + request.getContentLength()).c_str()));
    cgiEnv.push_back(
        strdup(("CONTENT_TYPE=" + request.getContentType()).c_str()));
    cgiEnv.push_back(strdup(("SCRIPT_FILENAME=" + route.getRoot() +
                             route.getPrefix() + "/" + scriptName)
                                .c_str()));
    cgiEnv.push_back(strdup(
        ("SCRIPT_NAME=" + route.getPrefix() + "/" + scriptName).c_str()));
    std::string pathInfo = request.getPathInfo(scriptName);
    cgiEnv.push_back(strdup(("PATH_INFO=" + pathInfo).c_str()));
    cgiEnv.push_back(
        strdup(("PATH_TRANSLATED=" + this->_getPathTranslated(pathInfo, route))
                   .c_str()));
    cgiEnv.push_back(strdup(("REQUEST_URI=" + request.getUri()).c_str()));
    cgiEnv.push_back(NULL);

    // Check for strdup failures
    for (size_t i = 0; i < cgiEnv.size() - 1; ++i)
        if (cgiEnv[ i ] == NULL)
            this->_cleanUp(
                NULL, cgiEnv.data()); // Free memory and throw exception 500

    // Log the environment variables
    for (size_t i = 0; i < cgiEnv.size() - 1; ++i)
        this->_logger.log(DEBUG,
                          "CGI Environment: " + std::string(cgiEnv[ i ]));
}

char *CgiResponseGenerator::_getCgiInterpreterPath(
    const std::string &scriptName, const IConfiguration &configuration) const
{
    // Extract file extension
    std::string fileExtension;
    size_t dotPos = scriptName.find_last_of('.');
    if (dotPos == std::string::npos) // No file extension
    {
        throw HttpStatusCodeException(BAD_REQUEST); // 400
    }
    fileExtension = scriptName.substr(dotPos + 1);

    // Set the interpreter path based on the file extension
    std::string interpreterPath;
    if (fileExtension == "php") // *.php
    {
        interpreterPath =
            configuration.getString("PhpCgiPath"); // e.g. /bin/php-cgi
    }
    else if (fileExtension == "py") // *.py
    {
        interpreterPath =
            configuration.getString("PythonCgiPath"); // e.g. /usr/bin/python3
    }
    else // Unsupported file extension
    {
        throw HttpStatusCodeException(NOT_IMPLEMENTED); // 501
    }

    // Return the interpreter path
    return strdup(interpreterPath.c_str());
}

char *CgiResponseGenerator::_getScriptPath(const std::string &scriptName,
                                           const IRoute &route) const
{
    // Get the root path
    std::string rootPath = route.getRoot();

    // Get the prefix
    std::string prefix = route.getPrefix();

    // Return the script path without the query string
    return strdup((rootPath + prefix + "/" + scriptName)
                      .c_str()); //  e.g. /path/to/script.php
}

std::string CgiResponseGenerator::_getPathTranslated(std::string &pathInfo,
                                                     const IRoute &route) const
{
    // PATH_TRANSLATED = location root + location prefix + PATH_INFO

    //  Get the location root path
    std::string rootPath = route.getRoot();

    // Get the location prefix
    std::string prefix = route.getPrefix();

    // Return the path translated
    return rootPath + prefix + pathInfo;
}

void CgiResponseGenerator::_cleanUp(char *cgiArgs[], char *cgiEnv[],
                                    int responsePipeFd[ 2 ],
                                    int bodyPipeFd[ 2 ], short option) const
{
    // Free args
    if (cgiArgs != NULL)
    {
        for (int i = 0; cgiArgs[ i ] != NULL; ++i)
        {
            free(cgiArgs[ i ]);
        }
    }

    // Free env
    if (cgiEnv != NULL)
    {
        for (int i = 0; cgiEnv[ i ] != NULL; ++i)
        {
            free(cgiEnv[ i ]);
        }
    }

    // Close response pipe
    if (responsePipeFd != NULL)
    {
        if ((option & KEEP_RESPONSE_READ_PIPE) == 0)
            close(responsePipeFd[ 0 ]);
        close(responsePipeFd[ 1 ]);
    }

    // Close body pipe
    if (bodyPipeFd != NULL)
    {
        if ((option & KEEP_BODY_WRITE_PIPE) == 0)
            close(bodyPipeFd[ 1 ]);
        close(bodyPipeFd[ 0 ]);
    }

    // Throw an exception
    if ((option & NO_THROW) == 0)
        throw HttpStatusCodeException(INTERNAL_SERVER_ERROR); // 500
}

// Path: srcs/CgiResponseGenerator.cpp
