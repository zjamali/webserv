#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const &request)
{
    // init servers 
    init_response();
    if (_responseStatus == 405 && __errorPages.find(405) == __errorPages.end())
    {
        std::cout << "\n---<kkkkkkkkkkkkkkkkkkkkkkkkkkkk\n";
        _finaleResponse = ResponseMethodNotAllowed();
        return;
    }

    _responseStatus = request.getRequestStatus();
    _method = request.getMethod();
    _httpVersion = request.getHttpVersion();
    _path = request.getPath();
    // generate response

    generateResponse();
}

void HttpResponse::init_response()
{
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

std::string HttpResponse::getStartLine() const
{

    if (_responseStatus == BAD_REQUEST) // bad request
    {
        return "HTTP/1.1 400 Bad Request\r\n";
    }
    else if (_responseStatus == 501)
    {
        return "HTTP/1.1 501 Not Implemented\r\n";
    }
    else if (_responseStatus == 505)
    { // 505 HTTP Version Not Supported

        return ("HTTP/1.1 505 HTTP Version Not Supported\r\n");
    }
    else if (_responseStatus == 200 && _path == "/")
    {
        return ("HTTP/1.1 200 OK\r\n");
    }
    else
        return ("HTTP/1.1 404 Not Found\r\n");
}

std::string HttpResponse::getHeaders() const
{
    std::string res;
    for (std::map<std::string, std::string>::const_iterator it = _responseHeaders.begin(); it != _responseHeaders.end(); it++)
    {
        res += it->first;
        res += ": ";
        res += it->second;
        res += "\r\n";
    }
    res += getLocalTime();
    return (res);
}

void HttpResponse::print()
{
    std::cout << "+++++++++++++++++++++ Responce BEGIN ++++++++++++++++++++\n";
    std::cout << _finaleResponse << std::endl;
    std::cout << "+++++++++++++++++++++ Responce END ++++++++++++++++++++\n";
}

std::string HttpResponse::getBody()
{

    if (_responseStatus == 400 || _responseStatus == 505 || _responseStatus == 501)
    {
        std::ifstream file("serverPages/" + std::to_string(_responseStatus) + ".html");
        if (file.is_open())
        {
            _responseHeaders["Content-Type"] = "text/html";
            _responseHeaders["Content-Length"] = "150";
            int size = 1500;
            char *buffer = new char[size];
            memset(buffer, '\0', size);
            file.read(buffer, size);

            file.close();
            _responseBody = buffer;
            delete[] buffer;
        }
    }
    else if (_responseStatus == 200 && _path == "/")
    {
        std::ifstream file("serverPages/200.html");
        if (file.is_open())
        {

            int size = 1500;
            char *buffer = new char[size];
            memset(buffer, '\0', size);
            file.read(buffer, size);
            file.close();
            _responseBody = buffer;
            delete[] buffer;
            _responseHeaders["Content-Type"] = "text/html";
            _responseHeaders["Content-Length"] = "150";
            //_responseHeaders["Content-Length"] = std::to_string(size);
        }
    }
    else if (_responseStatus == 200 && _path != "/")
    {
        std::ifstream file("serverPages/404.html");
        if (file.is_open())
        {
            int size = 1500;
            char *buffer = new char[size];
            memset(buffer, '\0', size);
            file.read(buffer, size);
            file.close();
            _responseBody = buffer;
            delete[] buffer;
            _responseHeaders["Content-Type"] = "text/html";
            _responseHeaders["Content-Length"] = "150";
        }
    }
    //_responseBody = "<html><head><title>400 Bad Request</title></head><body><center><h1>400 Bad Request</h1></center><hr><center>webserv/1.0</center></body></html>";
    return _responseBody;
}

std::string HttpResponse::generateResponse()
{
    std::string response;

    response += getStartLine();
    //response += getHeaders();
    // response += getLocalTime();
    __contentLength = "186";
    response += "Content-Type: " + __contentType + "\r\n";
    response += "Content-Lenght: " + __contentLength + "\r\n";
    response += "Server: " + __server + "\r\n";
    response += "\r\n";
    response += getBody();
    // add body

    // end the body
    response += "\r\n";
    _finaleResponse = response;
    
   _finaleResponse = ResponseOK();
    return (response);
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

std::string HttpResponse::ResponseOK() const
{
    //std::string const body = "<!DOCTYPE html>\n<html>\n<head>\n<title>Welcome to webserv!</title>\n</head>\n<body>\n<center>\n<h1>Welcome to webserv</h1>\n</center>\n<hr>\n<center>\n<p>webserv/1.0</p>\n</center>\n</body>\n</html>\n";
    std::string body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Welcome to webserv!</title> \r\n</head> \r\n<body>\r\n<center><h1>Welcome to webserv</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = getStartLine() + "Content-Type: text/html\r\nContent-Length: " + std::to_string(body.length()) + "\r\nConnection: Closed\r\nServer: webserv/1.0\r\nDate: " + getLocalTime();

    return (header + "\r\n\r\n" + body);
}
std::string const HttpResponse::ResponseBadRequest() const
{
    std::string const body = "<!DOCTYPE html>\n<html><head><title>400 Bad Request</title></ head><body><center><h1>400 Bad Request</h1></center><hr><center><p>webserv/1.0</p></center></body></html>";
    std::string const header = "HTTP/1.1 400 Bad Request" + CRLF_Combination + "Content-Type: text/html" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime() + CRLF_Combination;
    return (header + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseNotFound() const
{
    std::string const body = "<!DOCTYPE html><html><head><title>404 Not Found</title></ head><body><center><h1>404 Not Found</h1></center><hr><center><p>webserv/1.0</p></center></body></html>";
    std::string const header = "HTTP/1.1 404 Not Found" + CRLF_Combination + "Content-Type: text/html" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime() + CRLF_Combination;
    return (header + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseMethodNotAllowed() const
{
    std::string const body = "<!DOCTYPE html>\n<html>\n<head>\n<title>405 Method Not Allowed</title>\n</ head><body><center><h1>405 Method Not Allowed</h1></center><hr><center><p>webserv/1.0</p></center></body></html>";
    std::string const header = "HTTP/1.1 405 Method Not Allowed" + CRLF_Combination + "Content-Type: text/html" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime() + CRLF_Combination;
    return (header + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseHttpVersionNotSupported() const
{
    std::string const body = "<!DOCTYPE html><html><head><title>505 HTTP Version Not Supported</title></ head><body><center><h1>505 HTTP Version Not Supported</h1></center><hr><center><p>webserv/1.0</p></center></body></html>";
    std::string const header = "HTTP/1.1 505 HTTP Version Not Supported" + CRLF_Combination + "Content-Type: text/html" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime() + CRLF_Combination;
    return (header + CRLF_Combination + body);
}