
#include <fstream>
// #define stdin 0
// #define stdout 1
// using namespace std;

std::string LaunchCGI(location _location, std::string FilePath, Request _req)
{
    pid_t pid;
    int Ifd[2];
    int Ofd[2];

    std::string head;
    std::string body;
    std::string data;
    // int pipefd2[2];

    // int pipefd2[2];

    // int pipefd2[2];

    char buffer[CGI_BUFFER];
    std::string resContent;
    int ret;

    ret = 1;
    if (pipe(Ifd))
    {
        perror("[CGI ERROR] PIPE");
    }
    if (pipe(Ofd))
    {
        perror("[CGI ERROR] PIPE");
    }
    // if (pipe(pipefd2))
    // {
    //     perror("[CGI ERROR] PIPE");
    // }
    char *cgi_path = strdup(_location.getL_Fastcgi_Pass().c_str());
    char *argv[3];
    argv[0] = cgi_path;

    setenv("SERVER_SOFTWARE", "Webserv", 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("REQUEST_METHOD", _req.get_method().c_str(), 1);
    setenv("SERVER_PROTOCOL", _req.get_protocol().c_str(), 1);
    setenv("CONTENT_TYPE", _req.get_header_value("Content-Type:").c_str(), 1);
    setenv("SCRIPT_FILENAME", FilePath.c_str(), 1);
    setenv("REDIRECT_STATUS", std::to_string(_req.get_status()).c_str(), 1);
    argv[1] = getenv("SCRIPT_FILENAME");
    argv[2] = NULL;
    std::multimap<std::string, std::string> test = _req.get_header();

    if (_req.get_header().count("Cookie:") > 0)
    {
        std::multimap<std::string, std::string>::iterator cookiestring = test.find("Cookie:");
        setenv("HTTP_COOKIE", cookiestring->second.c_str(), 1);
    }
    if (_req.get_method() == "GET")
    {
        setenv("QUERY_STRING", _req.get_queary_string().c_str(), 1);
        setenv("CONTENT_LENGTH", "0", 1);// no body exist 
    }
    else if (_req.get_method() == "POST")
    {
        std::multimap<std::string, std::string>::iterator it = test.find("Content-Type:");

        if (it != _req.get_header().end())
        {
            if (it->second == "application/x-www-form-urlencoded")
            {
                data = _req.get_body();
                setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
            }
            else if (it->second.find("multipart/form-data") != npos)
            {
                setenv("CONTENT_TYPE", it->second.c_str(), 1);
                data = _req.get_bounday_body();
            }
            setenv("CONTENT_LENGTH", std::to_string(data.length()).c_str(), 1);
        }
    }

    extern char **environ;
    pid = fork();
    if (pid == CHILD)
    {
        close(Ifd[0]);
        // close(Ofd[1]);
        // if (_req.get_method() == "POST")
        //     if (dup2(Ofd[0], 0) < 1)
        //         std::cerr << "dup error" << std::endl;

        if (dup2(Ifd[1], 1) < 1)
            std::cerr << "dup2 error" << std::endl;

        // close(Ofd[0]);
        close(Ifd[1]);
        if ((execve(argv[0], argv, environ)) < 0)
            std::cerr << "error in execve" << std::endl;
        exit(1);
    }
    else
    {
        close(Ifd[1]);
        close(Ofd[0]);
        close(Ofd[1]);
        if (_req.get_method() == "POST")
        {
            int ret = write(Ofd[1], data.c_str(), data.length());
        }
        if (ret < 0)
            std::cerr << "Error in write\n";
        while (ret > 0)
        {
            memset(buffer, 0, CGI_BUFFER);
            ret = read(Ifd[0], buffer, CGI_BUFFER);
            resContent += buffer;
        }
        close(Ifd[0]);

        // wait(0);
    }
    // std::cout << "|" << resContent << "|" << std::endl;

    return resContent;
}