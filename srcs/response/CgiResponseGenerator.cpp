#include "../../includes/response/CgiResponseGenerator.hpp"

#define NO_THROW 0x1

CgiResponseGenerator::CgiResponseGenerator(ILogger &logger)
    : _logger(logger),
      _httpStatusCodeHelper(HttpStatusCodeHelper()) {}

CgiResponseGenerator::~CgiResponseGenerator() {}

// calls execve to execute the CGI script
// returns the read end of the pipe to read the response later without blocking
// Throws an exception if an error occurs
int CgiResponseGenerator::generateResponse(const IRoute &route, const IRequest &request, const IConfiguration &configuration, const std::string &scriptName)
{
    // Set cgi arguments
    std::vector<char *> cgiArgs;
    this->_setCgiArguments(scriptName, route, configuration, cgiArgs);

    // Set cgi environment variables
    std::vector<char *> cgiEnv;
    this->_setCgiEnvironment(scriptName, route, request, cgiEnv);

    int pipefd[2];
    if (pipe(pipefd) == -1)
        this->_cleanUp(cgiArgs.data(), cgiEnv.data()); // Free memory and throw exception 500

    pid_t pid = fork();
    if (pid == -1)
        this->_cleanUp(cgiArgs.data(), cgiEnv.data(), pipefd); // Free memory and throw exception 500

    else if (pid == 0) // child process
    {
        // Log the start of the CGI script execution
        this->_logger.log(DEBUG, "CGI script execution started with PID: " + std::to_string(getpid()));

        // stdout should write to pipe
        close(pipefd[0]);               // close read end
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
        close(pipefd[1]);               // close write end

        // Call execve
        execve(cgiArgs[0], cgiArgs.data(), cgiEnv.data());

        // If execve returns, an error occurred; free memory and exit
        this->_logger.log(ERROR, "CGI execve failed [" + std::to_string(errno) + ": " + std::string(strerror(errno)) + "] | terminating process with PID: " + std::to_string(getpid()));
        this->_cleanUp(cgiArgs.data(), cgiEnv.data(), pipefd, NO_THROW);
        exit(EXIT_FAILURE);
    }
    else // parent process
    {
        // Return the read end of the pipe to read the response later without blocking
        close(pipefd[1]); // close write end
        return pipefd[0]; // return read end
    }
    return (-1); // unreachable
}

void CgiResponseGenerator::_setCgiArguments(const std::string &scriptName, const IRoute &route, const IConfiguration &configuration, std::vector<char *> &cgiArgs)
{
    cgiArgs.push_back(this->_getCgiInterpreterPath(scriptName, configuration)); // Interpreter absolute path
    cgiArgs.push_back(this->_getScriptPath(scriptName, route));                 // script path: location block root path + URI(excl. query string)
    cgiArgs.push_back(NULL);

    // Check for strdup failures
    for (size_t i = 0; i < cgiArgs.size() - 1; ++i)
        if (cgiArgs[i] == NULL)
            this->_cleanUp(cgiArgs.data()); // Free memory and throw exception 500

    this->_logger.log(DEBUG, "CGI interpreter: " + std::string(cgiArgs[0]));
    this->_logger.log(DEBUG, "CGI script: " + std::string(cgiArgs[1]));
}

void CgiResponseGenerator::_setCgiEnvironment(const std::string &scriptName, const IRoute &route, const IRequest &request, std::vector<char *> &cgiEnv)
{
    cgiEnv.push_back(strdup(("REQUEST_METHOD=" + request.getMethodString()).c_str()));
    cgiEnv.push_back(strdup(("QUERY_STRING=" + request.getQueryString()).c_str()));
    cgiEnv.push_back(strdup(("CONTENT_LENGTH=" + request.getContentLength()).c_str()));
    cgiEnv.push_back(strdup(("CONTENT_TYPE=" + request.getContentType()).c_str()));
    cgiEnv.push_back(strdup(("SCRIPT_FILENAME=" + route.getRoot() + route.getPrefix() + "/" + scriptName).c_str()));
    cgiEnv.push_back(strdup(("SCRIPT_NAME=" + route.getPrefix() + "/" + scriptName).c_str()));
    std::string pathInfo = request.getPathInfo(scriptName);
    cgiEnv.push_back(strdup(("PATH_INFO=" + pathInfo).c_str()));
    cgiEnv.push_back(strdup(("PATH_TRANSLATED=" + this->_getPathTranslated(pathInfo, route)).c_str()));
    cgiEnv.push_back(strdup(("REQUEST_URI=" + request.getUri()).c_str()));
    cgiEnv.push_back(NULL);

    // Check for strdup failures
    for (size_t i = 0; i < cgiEnv.size() - 1; ++i)
        if (cgiEnv[i] == NULL)
            this->_cleanUp(NULL, cgiEnv.data()); // Free memory and throw exception 500

    // Log the environment variables
    for (size_t i = 0; i < cgiEnv.size() - 1; ++i)
        this->_logger.log(DEBUG, "CGI Environment: " + std::string(cgiEnv[i]));
}

char *CgiResponseGenerator::_getCgiInterpreterPath(const std::string &scriptName, const IConfiguration &configuration) const
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
        interpreterPath = configuration.getString("PhpCgiPath"); // e.g. /bin/php-cgi
    }
    else if (fileExtension == "py") // *.py
    {
        interpreterPath = configuration.getString("PythonCgiPath"); // e.g. /usr/bin/python3
    }
    else // Unsupported file extension
    {
        throw HttpStatusCodeException(NOT_IMPLEMENTED); // 501
    }

    // Return the interpreter path
    return strdup(interpreterPath.c_str());
}

char *CgiResponseGenerator::_getScriptPath(const std::string &scriptName, const IRoute &route) const
{
    // Get the root path
    std::string rootPath = route.getRoot();

    // Get the prefix
    std::string prefix = route.getPrefix();

    // Return the script path without the query string
    return strdup((rootPath + prefix + "/" + scriptName).c_str()); //  e.g. /path/to/script.php
}

std::string CgiResponseGenerator::_getPathTranslated(std::string &pathInfo, const IRoute &route) const
{
    // PATH_TRANSLATED = location root + location prefix + PATH_INFO

    //  Get the location root path
    std::string rootPath = route.getRoot();

    // Get the location prefix
    std::string prefix = route.getPrefix();

    // Return the path translated
    return rootPath + prefix + pathInfo;
}

void CgiResponseGenerator::_cleanUp(char *cgiArgs[], char *cgiEnv[], int pipefd[2], short option) const
{
    // Free args
    if (cgiArgs != NULL)
    {
        for (int i = 0; cgiArgs[i] != NULL; ++i)
        {
            free(cgiArgs[i]);
        }
    }

    // Free env
    if (cgiEnv != NULL)
    {
        for (int i = 0; cgiEnv[i] != NULL; ++i)
        {
            free(cgiEnv[i]);
        }
    }

    // Close pipe
    if (pipefd != NULL)
    {
        close(pipefd[0]);
        close(pipefd[1]);
    }

    // Throw an exception
    if (option != NO_THROW)
        throw HttpStatusCodeException(INTERNAL_SERVER_ERROR); // 500
}
// Path: srcs/CgiResponseGenerator.cpp
