#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const &request) : _request(request)
{
    __http = "";
    __statusCode = "";
    __statusDesciption = "";
    
    __connection = "Closed";
    __contentLength = "";
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

    _responseStatus = _request.getRequestStatus();
    _method = _request.getMethod();
    _httpVersion = _request.getHttpVersion();
    _path = request.getPath();
    // generate response

    generateResponse();
}

HttpResponse::~HttpResponse()
{
}

std::string HttpResponse::getLocalTime()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    memset(buffer, '\0', 80);
    strftime(buffer, 80, "%a, %d %h %Y %T GMT", timeinfo);
    return ("Date: " + std::string(buffer) + "\r\n");
}

std::string HttpResponse::getStartLine()
{

    if (_responseStatus == 400) // bad request
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
    else if (_responseStatus == 0 && _path == "/")
    {
        return ("HTTP/1.1 200 OK\r\n");
    }
    else
        return ("HTTP/1.1 404 Not Found\r\n");
}

std::string HttpResponse::getHeaders()
{
    std::string res;
    for (std::map<std::string, std::string>::iterator it = _responseHeaders.begin(); it != _responseHeaders.end(); it++)
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
    std::cout << _generatedResponse << std::endl;
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
    else if (_responseStatus == 0 && _path == "/")
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
    else if (_responseStatus == 0 && _path != "/")
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
    response += getHeaders();
    // response += getLocalTime();
    response += "\r\n";
    response += getBody();
    // add body

    // end the body
    response += "\r\n";
    _generatedResponse = response;
    return (response);
}