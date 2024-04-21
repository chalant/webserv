#include "../includes/CgiResponseGenerator.hpp"

// calls execve to execute the CGI script
// returns the read end of the pipe to read the response later without blocking
// returns -1 if an error occurred, and sets the response in that case to 500
int CgiResponseGenerator::generateResponse(const IRequest &request, IResponse &response)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        response.setErrorResponse(INTERNAL_SERVER_ERROR); // 500
        return -1;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        response.setErrorResponse(INTERNAL_SERVER_ERROR); // 500
        return -1;
    }
    else if (pid == 0) // child process
    {
        // stdout should write to pipe
        close(pipefd[0]);               // close read end
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe
        close(pipefd[1]);               // close write end

        // Get the script path and query string
        // e.g. /wikipedia.cgi?space
        std::string uri = request.getUri();
        std::string script = uri.substr(0, uri.find("?"));
        std::string query = uri.substr(uri.find("?") + 1);

        // Set the path to the executable
        std::string path = "../cgi-bin/" + script;

        // Create array of arguments
        char *args[] = {strdup(path.c_str()), strdup(query.c_str()), NULL};

        // Execute the CGI script
        execve(path.c_str(), args, (char *[]){NULL});

        // If execve returns, an error occurred, so free memory and return -1
        for (int i = 0; args[i] != NULL; ++i)
        {
            free(args[i]);
        }
        return -1;
    }
    else // parent process
    {
        // Return the read end of the pipe to read the response later without blocking
        close(pipefd[1]); // close write end
        return pipefd[0]; // return read end
    }
}

// Path: srcs/CgiResponseGenerator.cpp
