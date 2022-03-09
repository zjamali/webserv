#include "HttpRequest.hpp"

HttpRequest::HttpRequest(std::string const &request)
    : _requestIndex(0), _request(request), _requestStatus(0), _bodyExist(false)
{
    this->parseStartLine();
    this->checkRequestStartLine();
    this->parseHeaders();
    this->checkRequestkHeaders();
    if (_requestStatus == 0 && _method == "POST")
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

    _requestIndex = httpVersionEndPostion + 2; // +2 : \r\n end of line
}

void HttpRequest::parseHeaders()
{
    int headerEndIndex = _request.find("\r\n\r\n");
    while (_requestIndex < headerEndIndex)
    {
        int separator = _request.find(":", _requestIndex);
        int endLine = _request.find("\r\n", _requestIndex);
        _headers[_request.substr(_requestIndex, separator - _requestIndex) /* separator - i : to doesn't include separator  */] = _request.substr(separator + 2, endLine - separator - 2); // separator + 2 : first charcter of value ;endLine - separator - 2 : index of last character of headers-entities
        _requestIndex = endLine + 2;
    }
}

void HttpRequest::checkRequestStartLine()
{
    if (_method != "GET" && _method != "DELETE" && _method != "POST")
        _requestStatus = 501; // 501 Not Implemented
    if (_httpVersion != "HTTP/1.1"  && _httpVersion != "HTTP/1.1")
        _requestStatus = 505; // 505 HTTP Version Not Supported
}

void HttpRequest::checkRequestkHeaders()
{
}

void HttpRequest::parseRequestBody()
{
    _requestBody = _request.c_str() + _requestIndex; // skip + 4 : /r/n/r/n
    std::cout << "\n-------------------------  BODY BEGIN -------------------------\n";
    std::cout << _requestBody << "\n";
    std::cout << "\n-------------------------  BODY END -------------------------\n";

    // std::map<std::string, std::string>::iterator it = _headers.find("Content-Type");
    if (_headers.find("Content-Type") == _headers.end() ||_headers["Content-Type"].empty())
        _bodyExist = false;
    else
        _bodyExist = true;

    if (_bodyExist)
    {
        
    }
    // find boundry key and get value of boundry
    //_boundary = _headers["Content-Type"].c_str() + _headers["Content-Type"].find("boundary=") + strlen("boundary=");
}

void HttpRequest::parseBodyparts()
{
    long index = 0;
    int contentDispositionStart;
    int contentDispositionEnd;
    std::string contentType;
    std::string name;
    std::string data;
    while ((index = _requestBody.find("--" + _boundary + "\r\n", index + 1)))
    {
        if (index == (long)std::string::npos)
            break;
        contentDispositionStart = _requestBody.find("Content-Disposition:", index + 1) + strlen("Content-Disposition: ");
        contentDispositionEnd = _requestBody.find(";", contentDispositionStart);
        // std::cout <<"-> dispostion : {" << _requestBody.substr(contentDispositionStart, contentDispositionEnd - contentDispositionStart) <<"}\n";
    }
}

void HttpRequest::print() const
{
    std::cout << "\n-------------------------  REQUEST BEGIN -------------------------\n";
    std::cout << "request status " << _requestStatus << "\n";
    std::cout << "method  :  {" << _method << "}\n";
    std::cout << "path  :  {" << _path << "}\n";
    std::cout << "http version  :  {" << _httpVersion << "}\n";
    std::cout << "body : " << (_bodyExist?"exist\n":"not exist\n") ;

    std::cout << "**** headers ****\n";
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
    {
        std::cout << "{" << it->first << "} : {" << it->second << "}\n";
    }

    std::cout << "-------------------------  REQUEST END   -------------------------\n";
}