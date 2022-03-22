#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const &request)
{
    // init servers
    init_response();
    _responseStatus = request.getRequestStatus();
    _method = request.getMethod();
    _httpVersion = request.getHttpVersion();
    _path = request.getPath();

    std::map<std::string, std::string> _headers = request.getHedaers(); 
    if (_headers.find("Content-Type:") != _headers.end())
        __contentType = request.getHedaers()["Content-Type:"];
    // generate response

    _finaleResponse = generateResponse("/Users/zjamali/Desktop/webserv/www", "upload"); // get config obj ; root
}

void HttpResponse::init_response()
{
    _errorHtml = false;
    CRLF_Combination = std::string(CRLF);
    __http = "";
    __statusCode = "";
    __statusDesciption = "";
    __connection = "Closed";
    __contentLength = "150";
    __contentType = "text/html";
    __Date = "";
    __server = "Webserv/1.0";

    //// Successful responses
    __codes[200] = "OK";
    __codes[201] = "Created";
    __codes[202] = "Accepted";
    __codes[203] = "Non-Authoritative Information";
    __codes[204] = "No Content";
    __codes[205] = "Reset Content";
    __codes[206] = "Partial Content";
    __codes[207] = "Multi-Status";
    __codes[208] = "208 Already Reported";
    __codes[226] = "226 IM Used";
    //// Redirection messages
    __codes[300] = "Multiple Choice";
    __codes[301] = "Moved Permanently";
    __codes[302] = "Found";
    __codes[303] = "See Other";
    __codes[304] = "Not Modified";
    __codes[305] = "Use Proxy";
    __codes[306] = "unused";
    __codes[307] = "Temporary Redirect";
    __codes[308] = "Permanent Redirect";
    //// Client error responses
    __codes[400] = "Bad Request";
    __codes[401] = "Unauthorized";
    __codes[402] = "Payment Required";
    __codes[403] = "Forbidden";
    __codes[404] = "Not Found";
    __codes[405] = "Method Not Allowed";
    __codes[406] = "Not Acceptable";
    __codes[407] = "Proxy Authentication Required";
    __codes[408] = "Request Timeout";
    __codes[409] = "Conflict";
    __codes[410] = "Gone";
    __codes[411] = "Length Required";
    __codes[412] = "Precondition Failed";
    __codes[413] = "Payload Too Large";
    __codes[414] = "URI Too Long";
    __codes[415] = "Unsupported Media Type";
    __codes[416] = "Range Not Satisfiable";
    __codes[417] = "Expectation Failed";
    __codes[418] = "I'm a Teapot";
    __codes[421] = "Misdirected Request";
    __codes[422] = "Unprocessable Entity";
    __codes[423] = "Locked";
    __codes[424] = "Failed Dependency";
    __codes[425] = "Too Early";
    __codes[426] = "Upgrade Required";
    __codes[428] = "Precondition Required";
    __codes[429] = "Too Many Requests";
    __codes[431] = "Request Header Fields Too Large";
    __codes[451] = "Unavailable For Legal Reasons";
    //// Server error responses
    __codes[500] = "Internal Server Error";
    __codes[501] = "Not Implemented";
    __codes[502] = "Bad Gateway";
    __codes[503] = "Service Unavailable";
    __codes[504] = "Gateway Timeout";
    __codes[505] = "HTTP Version Not Supported";
    __codes[506] = "Variant Also Negotiates";
    __codes[507] = "Insufficient Storage";
    __codes[508] = "Loop Detected";
    __codes[510] = "Not Extended";
    __codes[511] = "Network Authentication Required";
}

HttpResponse::~HttpResponse()
{
}

std::string HttpResponse::getLocalTime() const
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    memset(buffer, '\0', 80);
    strftime(buffer, 80, "%a, %d %h %Y %T GMT", timeinfo);
    return (std::string(buffer));
}

std::string HttpResponse::generateStartLine(unsigned int status_code) const
{

    if (status_code == BAD_REQUEST) // bad request
    {
        return "HTTP/1.1 400 Bad Request\r\n";
    }
    else if (status_code == NOT_IMPLLIMENTED)
    {
        return "HTTP/1.1 501 Not Implemented\r\n";
    }
    else if (status_code == HTTP_VERSION_NOT_SUPPORTED)
    { // 505 HTTP Version Not Supported

        return ("HTTP/1.1 505 HTTP Version Not Supported\r\n");
    }
    else if (status_code == OK)
    {
        return ("HTTP/1.1 200 OK\r\n");
    }
    else
        return ("HTTP/1.1 404 Not Found\r\n");
}

std::string HttpResponse::generateHeader(unsigned int const status_code, unsigned int body_lenght, std::string const content_type)
{
    std::string header;
    header += generateStartLine(status_code);
    header += "Content-Type: " + content_type + "\r\n";
    header += "Content-Lenght: " + std::to_string(body_lenght) + "\r\n";
    header += "Server: " + __server + "\r\n";
    header += "\r\n";
    return header;
}

void HttpResponse::print()
{
    std::cout << "+++++++++++++++++++++ Responce BEGIN ++++++++++++++++++++\n";
    std::cout << _finaleResponse << std::endl;
    std::cout << "+++++++++++++++++++++ Responce END ++++++++++++++++++++\n";
}

std::string HttpResponse::generateBody()
{
    return " ";
}

std::string const HttpResponse::defaultServerPages(unsigned int statusCode) const
{
    if (statusCode == OK)
        return ResponseOK();
    else if (statusCode == BAD_REQUEST)
        return ResponseBadRequest();
    else if (statusCode == NOT_FOUND)
        return ResponseNotFound();
    else if (statusCode == METHOD_NOT_ALLOWED)
        return ResponseMethodNotAllowed();
    else if (statusCode == HTTP_VERSION_NOT_SUPPORTED)
        return ResponseHttpVersionNotSupported();
    else if (statusCode == NOT_IMPLLIMENTED)
        return ResponseMethodNotAllowed();
    else
        return ResponseNotFound();
}

std::string const HttpResponse::generateErrorResponse(unsigned int errorCode)
{
    if (_errorHtml)
    {
        if (__errorPages.find(errorCode) == __errorPages.end())
            return defaultServerPages(errorCode);
        else
        {
            std::string body;
            // get Html page path
            std::string pagePath = __errorPages[errorCode];
            struct stat sb;
            if (!stat(pagePath.c_str(), &sb) && S_ISREG(sb.st_mode))
            {
                int fd = open(pagePath.c_str(), O_RDONLY);
                if (fd > 0)
                {
                    char buffer[(int)sb.st_size];
                    int readedBytes;
                    while ((readedBytes = read(fd, buffer, sb.st_size)) > 0)
                    {
                        buffer[readedBytes] = '\0';
                        body += buffer;
                        if (body.length() >= (unsigned int)sb.st_size)
                            break;
                    }
                    close(fd);
                }
            }
            return (generateHeader(errorCode, body.length(), __contentType) + CRLF_Combination + CRLF_Combination + body);
        }
    }
    else
    {
        return defaultServerPages(errorCode);
    }
}

std::string readFile(std::string const &pagePath)
{
    struct stat sb;
    std::string body;
    stat(pagePath.c_str(), &sb);
    int fd = open(pagePath.c_str(), O_RDONLY);
    if (fd > 0)
    {
        char buffer[(int)sb.st_size];
        int readedBytes;
        while ((readedBytes = read(fd, buffer, sb.st_size)) > 0)
        {
            buffer[readedBytes] = '\0';
            body += buffer;
            if (body.length() >= (unsigned int)sb.st_size)
                break;
        }
        close(fd);
    }
    return body;
}

std::string HttpResponse::generateResponse(std::string const &root /*or location*/, std::string const &uploadPath)
{
    std::string response;
    std::string header;
    std::string body;

    std::cout << "--------+ path : " << _path << "\n";
    if (_responseStatus != 200)
    {
        return generateErrorResponse(_responseStatus);
    }

    (void)root;
    (void)uploadPath;

    /// check the path is correct and file request exist
    ///
    struct stat sb;
    if (stat(root.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        /// search for index
        if (_path == "/")
        {
            if (stat(std::string(root + "/" + "index.html").c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
                body = readFile(std::string(root + "/" + "index.html")); 
            else
                return generateErrorResponse((_responseStatus = 404));
        }
        else
        {
            
            if (_path.find(".") != std::string::npos)
            {
                std::string filetype = _path.substr(_path.find(".", _path.length() - 5));
                if (filetype == ".css")
                    __contentType = "text/css; charset=UTF-8";
                if (filetype == ".js")
                    __contentType = "text/javascript; charset=UTF-8";
                if (filetype == ".jpg" || filetype == ".jpeg")
                    __contentType = "image/jpeg";
                if (filetype == ".js")
                    __contentType = "text/javascript";
                if (filetype == ".ico")
                    __contentType = "image/vnd.microsoft.icon";
                if (filetype == ".svg")
                    __contentType = "image/svg+xml";

            }
            body = readFile(root + _path);
        }
    }
    else
        return generateErrorResponse((_responseStatus = 404));
    ///
    header = generateStartLine(_responseStatus);
    header += generateHeader(_responseStatus, body.length(), __contentType);
    // add body

    // end the body

    return (header + CRLF_Combination + CRLF_Combination + body);
}

std::string HttpResponse::ResponseOK() const
{
    std::string body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Welcome to webserv!</title> \r\n</head> \r\n<body>\r\n<center><h1>Welcome to webserv</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 200 OK" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8\r\nContent-Length: " + std::to_string(body.length()) + "\r\nConnection: Closed\r\nServer: webserv/1.0\r\nDate: " + getLocalTime();

    return (header + CRLF_Combination + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseBadRequest() const
{
    std::string const body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>400 Bad Request</title> \r\n</head> \r\n<body>\r\n<center><h1>400 Bad Request</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 400 Bad Request" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime();
    return (header + CRLF_Combination + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseNotFound() const
{
    std::string const body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>404 Not Found</title> \r\n</head> \r\n<body>\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 404 Not Found" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime();
    return (header + CRLF_Combination + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseMethodNotAllowed() const
{
    std::string const body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>405 Method Not Allowed</title> \r\n</head> \r\n<body>\r\n<center><h1>405 Method Not Allowed</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 405 Method Not Allowed" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime();
    return (header + CRLF_Combination + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseHttpVersionNotSupported() const
{
    std::string const body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>505 HTTP Version Not Supported</title> \r\n</head> \r\n<body>\r\n<center><h1>505 HTTP Version Not Supported</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 505 HTTP Version Not Supported" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime();
    return (header + CRLF_Combination + CRLF_Combination + body);
}

bool upload(std::string const &path, std::string const &filename, std::string const &data)
{
    struct stat stats;

    if (stat(path.c_str(), &stats) == 0 && S_ISDIR(stats.st_mode))
    {
        std::ofstream outfile(path + "/" + filename);
        outfile << data;
        outfile.close();
        return 0;
    }
    else
    {
        std::cout << "file not created" << std::endl;

        return 1;
    }
}