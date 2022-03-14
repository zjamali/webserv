#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const &request) : _request(request)
{
    _responseStatus = _request.getRequestStatus();
    _responseHeaders["Server"] = "webserv/1.0";
    //_responseHeaders["Content-Type"] = "text/html";
    //_responseHeaders["Content-Length"] = "150";
    _responseHeaders["Connection"] = "close";
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