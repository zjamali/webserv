#include "HttpRequest.hpp"

HttpRequest::HttpRequest(std::string const &request)
    : _requestIndex(0), _request(request), _requestStatus(0), _bodyExist(false)
{
    this->parseStartLine();
    this->checkRequestStartLine();
    this->parseHeaders();
    this->parseRequestBody();
}
HttpRequest::~HttpRequest()
{
}

void HttpRequest::parseStartLine()
{
    int methodEndPostion = _request.find(" ");
    _method = _request.substr(0, methodEndPostion);
    methodEndPostion++;
    int pathEndPostion = _request.find(" ", methodEndPostion);
    _path = _request.substr(methodEndPostion, pathEndPostion - methodEndPostion);
    pathEndPostion++;
    int httpVersionEndPostion = _request.find("\r\n", pathEndPostion);
    _httpVersion = _request.substr(pathEndPostion, httpVersionEndPostion - pathEndPostion);
    
    if (_path.find("?") != std::string::npos)
    {
        /// get queries and change path
        int pathEnd = _path.find("?");
        _queriesData = _path.substr(pathEnd + 1);
        std::string newPath = _path.substr(0,pathEnd);
        _path = newPath;
        _path = newPath;
        // get queries
        std::cout << "queries : " << _queriesData<< "\n";
    }
    _requestIndex = httpVersionEndPostion + 2; // +2 : \r\n end of line
}

void HttpRequest::parseHeaders()
{
    int headerEndIndex = _request.find("\r\n\r\n");
    while (_requestIndex < headerEndIndex)
    {
        int separator = _request.find(":", _requestIndex);
        int endLine = _request.find("\r\n", _requestIndex);
        std::string headerKey = _request.substr(_requestIndex, separator - _requestIndex);
        this->checkRequestkHeaders(headerKey);
        _headers[_request.substr(_requestIndex, separator - _requestIndex)] = /* separator - i : to doesn't include separator  */
            _request.substr(separator + 2, endLine - separator - 2);          // separator + 2 : first charcter of value ;endLine - separator - 2 : index of last character of headers-entities
        _requestIndex = endLine + 2;
    }
    if (_headers.find("Host") == _headers.end() && _headers.find("host") == _headers.end()) // no Host header found
        _requestStatus = 400;                                                               // bad request
}

void HttpRequest::checkRequestStartLine()
{
    if (_method != "GET" && _method != "DELETE" && _method != "POST")
        _requestStatus = 501; // 501 Not Implemented
    if (_httpVersion != "HTTP/1.1" && _httpVersion != "HTTP/1.1")
        _requestStatus = 505; // 505 HTTP Version Not Supported
    if (_method.size() != 3)
        _requestStatus = 400; // 400 Bad Request
}

void HttpRequest::checkRequestkHeaders(std::string const &headerKey)
{
    if (headerKey == "Host")
        if (_headers.find("Host") != _headers.end())
            _requestStatus = 400; // bad request
    if (headerKey == "host")
        if (_headers.find("host") != _headers.end())
            _requestStatus = 400;
}

void HttpRequest::parseRequestBody()
{
    _requestBody = _request.c_str() + _requestIndex; // skip + 4 : /r/n/r/n
    std::cout << "\n-------------------------  BODY BEGIN -------------------------\n";
    std::cout << _requestBody << "\n";
    std::cout << "\n-------------------------  BODY END -------------------------\n";

    if (_requestBody == "\r\n")
        _bodyExist = false;
    else
        _bodyExist = true;
}

/*
**  getters
*/
std::string HttpRequest::getMethod() const
{
    return _method;
}
std::string HttpRequest::getHttpVersion() const
{
    return _httpVersion;
}
std::string HttpRequest::getPath() const
{
    return _path;
}
int HttpRequest::getRequestStatus() const
{
    return _requestStatus;
}
std::map<std::string, std::string> HttpRequest::getHedaers() const
{
    return _headers;
}
std::map<std::string, std::string> HttpRequest::getQueries() const
{
    return _queries;
}

void HttpRequest::print() const
{
    std::cout << "\n-------------------------  REQUEST BEGIN -------------------------\n";
    std::cout << "request status " << _requestStatus << "\n";
    std::cout << "method  :  {" << _method << "}\n";
    std::cout << "path  :  {" << _path << "}\n";
    std::cout << "http version  :  {" << _httpVersion << "}\n";
    std::cout << "body : " << (_bodyExist ? "exist\n" : "not exist\n");

    std::cout << "**** headers ****\n";
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
    {
        std::cout << "{" << it->first << "} : {" << it->second << "}\n";
    }

    std::cout << "-------------------------  REQUEST END   -------------------------\n";
}