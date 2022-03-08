#include "HttpRequest.hpp"

HttpRequest::HttpRequest(std::string const &request) : _requestIndex(0), _request(request)
{
    this->parseStartLine();
    this->parseHeaders();
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

    _requestIndex = httpVersionEndPostion + 2;// +2 : \r\n end of line
}

void HttpRequest::parseHeaders()
{
    int headerEndIndex = _request.find("\r\n\r\n");
    while (_requestIndex < headerEndIndex)
    {
        int separator = _request.find(":", _requestIndex);
        int endLine = _request.find("\r\n", _requestIndex);
        _headers[_request.substr(_requestIndex ,separator - _requestIndex)/* separator - i : to doesn't include separator  */ ] 
            = _request.substr(separator + 2,endLine - separator - 2); // separator + 2 : first charcter of value ;endLine - separator - 2 : index of last character of headers-entities
        _requestIndex = endLine + 2; 
    }

}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::print() const
{
    std::cout << "\n-------------------------  REQUEST BEGIN -------------------------\n";
    std::cout << "method  :  {" << _method << "}\n"; 
    std::cout << "path  :  {" << _path << "}\n"; 
    std::cout << "http version  :  {" << _httpVersion << "}\n"; 

    std::cout << "**** headers ****\n";
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
    {
        std::cout << "{" <<it->first << "} : {" << it->second << "}\n";
    }

    std::cout << "-------------------------  REQUEST END   -------------------------\n";
}