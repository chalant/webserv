#include "../../includes/response/CgiResponseGenerator.hpp"
#include "../../includes/exception/WebservExceptions.hpp"
#include "../../includes/utils/Converter.hpp"
#include <cerrno>
#include <cstdlib>

#define NO_THROW 0x1 // Do not throw an exception
#define KEEP_CGI_OUTPUT_PIPE_READ_END                                          \
    0x2 // Do no close the read end of the CGI Output pipe
#define KEEP_CGI_INPUT_PIPE_WRITE_END                                          \
    0x4             // Do not close the write end of the CGI Input pipe
#define READ_END 0  // Read end of a pipe
#define WRITE_END 1 // Write end of a pipe

CgiResponseGenerator::CgiResponseGenerator(ILogger &logger)
    : m_logger(logger), m_http_status_code_helper(HttpStatusCodeHelper())
{
}

CgiResponseGenerator::~CgiResponseGenerator() {}

// calls execve to execute the CGI script
// returns the cgi process Info (pid, read end of the CGI Output pipe, write end
// of the CGI Input pipe) Throws an exception if an error occurs
Triplet_t CgiResponseGenerator::generateResponse(const IRoute &route,
                                                 const IRequest &request,
                                                 IResponse &response,
                                                 IConfiguration &configuration)
{
    // void the unused parameters
    (void)response;
    (void)configuration;

    // Get the CGI script path
    const std::string cgi_script = route.getCgiScript();

    // Set the Script path
    std::string uri = request.getUri();
    size_t last_slash = uri.find_last_of('/');
    size_t question_mark = uri.find('?');
    std::string script = uri.substr(last_slash + 1, question_mark - last_slash - 1);

    // Set cgi arguments
    std::vector<char *> cgi_args;
    m_setCgiArguments(cgi_script, script, route, cgi_args);

    // Set cgi environment variables
    std::vector<char *> cgi_env;
    m_setCgiEnvironment(script, route, request, cgi_env);

    // Create a pipe to send the response from the CGI script to the server
    int cgi_output_pipe_fd[ 2 ];
    if (pipe(cgi_output_pipe_fd) == -1)
        // pipe failed
        m_cleanUp(cgi_args.data(),
                  cgi_env.data()); // Free memory and throw exception 500

    // Create a pipe to send the request body from the server to the CGI script
    int cgi_input_pipe_fd[ 2 ];
    if (pipe(cgi_input_pipe_fd) == -1)
        // pipe failed
        m_cleanUp(cgi_args.data(), cgi_env.data(),
                  cgi_output_pipe_fd); // Free memory and throw exception 500

    // Fork a child process
    pid_t pid = fork();
    if (pid == -1)
        // fork failed
        m_cleanUp(cgi_args.data(), cgi_env.data(), cgi_output_pipe_fd,
                  cgi_input_pipe_fd); // Free memory and throw exception 500

    else if (pid == 0) // child process
    {
        m_logger.log(DEBUG,
                     "Forked a child process to execute the CGI script PID: " +
                         Converter::toString(getpid()) +
                         " Parent PID: " + Converter::toString(getppid()));
        // stdin should read from CGI Input pipe
        close(cgi_input_pipe_fd[ WRITE_END ]); // close write end
        dup2(cgi_input_pipe_fd[ READ_END ],
             STDIN_FILENO);                   // redirect stdin to pipe
        close(cgi_input_pipe_fd[ READ_END ]); // close read end

        // stdout should write to CGI Output pipe
        close(cgi_output_pipe_fd[ READ_END ]); // close read end
        dup2(cgi_output_pipe_fd[ WRITE_END ],
             STDOUT_FILENO);                    // redirect stdout to pipe
        close(cgi_output_pipe_fd[ WRITE_END ]); // close write end

        // Call execve
        execve(cgi_args[ 0 ], cgi_args.data(), cgi_env.data());
        m_logger.log(ERROR, "Execve failed: " + std::string(strerror(errno)));
        // If execve returns, an error occurred; free memory and exit
        m_cleanUp(cgi_args.data(), cgi_env.data(), cgi_output_pipe_fd,
                  cgi_input_pipe_fd, NO_THROW);
        exit(EXIT_FAILURE);
    }

    else // parent process
    {
        // Log the new CGI process ID
        m_logger.log(DEBUG, "New CGI process ID: " + Converter::toString(pid));

        // Set the read end of the Cgi Output Pipe to non-blocking
        fcntl(cgi_output_pipe_fd[ 0 ], F_SETFL, O_NONBLOCK);

        // Set the write end of the CGI Input pipe to non-blocking
        fcntl(cgi_input_pipe_fd[ 1 ], F_SETFL, O_NONBLOCK);

        // Free memory and keep the write end of the CGI Input pipe open and the
        // read end of the CGI Output pipe open
        m_cleanUp(cgi_args.data(), cgi_env.data(), cgi_output_pipe_fd,
                  cgi_input_pipe_fd,
                  NO_THROW | KEEP_CGI_OUTPUT_PIPE_READ_END |
                      KEEP_CGI_INPUT_PIPE_WRITE_END);

        // Log the Cgi info
        m_logger.log(VERBOSE, "Returning CGI info tuple; PID: " +
                                  Converter::toString(pid) +
                                  " CGI output pipe Read end: " +
                                  Converter::toString(cgi_output_pipe_fd[ 0 ]) +
                                  " CGI input pipe Write end: " +
                                  Converter::toString(cgi_input_pipe_fd[ 1 ]));

        // Return the read end of the pipe to read the response later without
        // blocking
        return std::make_pair(pid,
                              std::make_pair(cgi_output_pipe_fd[ READ_END ],
                                             cgi_input_pipe_fd[ WRITE_END ]));
    }

    return std::make_pair(-1, std::make_pair(-1, -1)); // unreachable code
}
#include <iostream>
void CgiResponseGenerator::m_setCgiArguments(
    const std::string &cgi_script, const std::string &script, const IRoute &route, std::vector<char *> &cgi_args)
{
    cgi_args.push_back(strdup(cgi_script.c_str())); // Interpreter absolute path
std::cout << "cgi_script: " << cgi_script << std::endl;
    cgi_args.push_back(m_getScriptPath(
        script, route)); // script path: location block root path +
                                        // URI(excl. query string)
    cgi_args.push_back(NULL);

    // Check for strdup failures
    for (size_t i = 0; i < cgi_args.size() - 1; ++i)
        if (cgi_args[ i ] == NULL)
            m_cleanUp(cgi_args.data()); // Free memory and throw exception 500

    m_logger.log(DEBUG, "CGI interpreter: " + std::string(cgi_args[ 0 ]));
    m_logger.log(DEBUG, "CGI script: " + std::string(cgi_args[ 1 ]));
}

void CgiResponseGenerator::m_setCgiEnvironment(const std::string &script,
                                               const IRoute &route,
                                               const IRequest &request,
                                               std::vector<char *> &cgi_env)
{
    cgi_env.push_back(
        strdup(("REQUEST_METHOD=" + request.getMethodString()).c_str()));
    cgi_env.push_back(
        strdup(("QUERY_STRING=" + request.getQueryString()).c_str()));
    cgi_env.push_back(
        strdup(("CONTENT_LENGTH=" + request.getContentLength()).c_str()));
    cgi_env.push_back(
        strdup(("CONTENT_TYPE=" + request.getContentType()).c_str()));
    std::string script_filename = route.getRoot() + route.getPath();
    if (script_filename.back() != '/')
        script_filename += "/";
    script_filename += script;
    cgi_env.push_back(strdup(
        ("SCRIPT_FILENAME=" + script_filename).c_str()));
    cgi_env.push_back(strdup(("SCRIPT_NAME=" + script).c_str()));
    std::string path_info = request.getPathInfo(script);
    cgi_env.push_back(strdup(("PATH_INFO=" + path_info).c_str()));
    cgi_env.push_back(strdup(
        ("PATH_TRANSLATED=" + m_getPathTranslated(path_info, route)).c_str()));
    cgi_env.push_back(strdup(("REQUEST_URI=" + request.getUri()).c_str()));
    cgi_env.push_back(NULL);

    // Check for strdup failures
    for (size_t i = 0; i < cgi_env.size() - 1; ++i)
        if (cgi_env[ i ] == NULL)
            m_cleanUp(NULL,
                      cgi_env.data()); // Free memory and throw exception 500

    // Log the environment variables
    for (size_t i = 0; i < cgi_env.size() - 1; ++i)
        m_logger.log(DEBUG, "CGI Environment: " + std::string(cgi_env[ i ]));
}

// Not used, must be defined in configuration file
// e.g. cgi_script /usr/bin/python3
char *CgiResponseGenerator::m_getCgiInterpreterPath(
    const std::string &script_name, const IConfiguration &configuration) const
{
    // Extract file extension
    std::string file_extension;
    size_t dot_pos = script_name.find_last_of('.');
    if (dot_pos == std::string::npos) // No file extension
    {
        throw HttpStatusCodeException(BAD_REQUEST); // 400
    }
    file_extension = script_name.substr(dot_pos + 1);

    // Set the interpreter path based on the file extension
    std::string interpreter_path;
    if (file_extension == "php") // *.php
    {
        interpreter_path =
            configuration.getString("php_cgi_path"); // e.g. /bin/php-cgi
    }
    else if (file_extension == "py") // *.py
    {
        interpreter_path =
            configuration.getString("python_cgi_path"); // e.g. /usr/bin/python3
    }
    else // Unsupported file extension
    {
        throw HttpStatusCodeException(NOT_IMPLEMENTED); // 501
    }

    // Return the interpreter path
    return strdup(interpreter_path.c_str());
}

char *CgiResponseGenerator::m_getScriptPath(const std::string &script_name,
                                            const IRoute &route) const
{
    // Get the root path
    std::string root_path = route.getRoot();

    // Get the prefix
    std::string prefix = route.getPath();

    // Append a slash if the prefix does not end with a slash
    if (prefix[ prefix.size() - 1 ] != '/')
        prefix += '/';

    // Return the script path without the query string
    return strdup((root_path + prefix + script_name)
                      .c_str()); //  e.g. /path/to/script.php
}

std::string CgiResponseGenerator::m_getPathTranslated(std::string &path_info,
                                                      const IRoute &route) const
{
    // PATH_TRANSLATED = location root + location prefix + PATH_INFO

    //  Get the location root path
    std::string root_path = route.getRoot();

    // Get the location prefix
    std::string prefix = route.getPath();

    // Return the path translated
    return root_path + prefix + path_info;
}

void CgiResponseGenerator::m_cleanUp(char *cgi_args[], char *cgi_env[],
                                     int cgi_output_pipe_fd[ 2 ],
                                     int cgi_input_pipe_fd[ 2 ],
                                     short option) const
{
    // Free args
    if (cgi_args != NULL)
    {
        for (int i = 0; cgi_args[ i ] != NULL; ++i)
        {
            free(cgi_args[ i ]);
        }
    }

    // Free env
    if (cgi_env != NULL)
    {
        for (int i = 0; cgi_env[ i ] != NULL; ++i)
        {
            free(cgi_env[ i ]);
        }
    }

    // Close CGI Output pipe
    if (cgi_output_pipe_fd != NULL)
    {
        if ((option & KEEP_CGI_OUTPUT_PIPE_READ_END) == 0)
            close(cgi_output_pipe_fd[ 0 ]);
        close(cgi_output_pipe_fd[ 1 ]);
    }

    // Close CGI Input pipe
    if (cgi_input_pipe_fd != NULL)
    {
        if ((option & KEEP_CGI_INPUT_PIPE_WRITE_END) == 0)
            close(cgi_input_pipe_fd[ 1 ]);
        close(cgi_input_pipe_fd[ 0 ]);
    }

    // Throw an exception
    if ((option & NO_THROW) == 0)
        throw HttpStatusCodeException(INTERNAL_SERVER_ERROR); // 500
}

// Path: srcs/CgiResponseGenerator.cpp
