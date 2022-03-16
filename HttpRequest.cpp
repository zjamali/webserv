#include "HttpRequest.hpp"

HttpRequest::HttpRequest(std::string const &request)
    : _requestIndex(0), _request(request), _requestStatus(0), _bodyExist(false)
{
    this->parseStartLine();

    this->parseHeaders();
    if (_method == "POST")
        this->parseRequestBody();
}
HttpRequest::~HttpRequest()
{
}

/*
 *   Request header
 */
void HttpRequest::parseStartLine()
{
    std::string startLine = _request.substr(0,_request.find("\r\n"));

    int methodEndPostion = _request.find(" ");
    _method = _request.substr(0, methodEndPostion);
    methodEndPostion++;
    int pathEndPostion = _request.find(" ", methodEndPostion);
    _path = _request.substr(methodEndPostion, pathEndPostion - methodEndPostion);
    pathEndPostion++;
    int httpVersionEndPostion = _request.find("\r\n", pathEndPostion);
    _httpVersion = _request.substr(pathEndPostion, httpVersionEndPostion - pathEndPostion);
    this->checkRequestStartLine(startLine);
    if (_path.find("?") != std::string::npos)
    {
        /// get queries and change path
        int pathEnd = _path.find("?");
        _quereyData = _path.substr(pathEnd + 1);
        std::string newPath = _path.substr(0, pathEnd);
        _path = newPath;
        // get queries
        if (_quereyData.find("=") != std::string::npos)
            _querey[_quereyData.substr(0, _quereyData.find("="))] = _quereyData.substr(_quereyData.find("=") + 1);
        else
            _querey[_quereyData] = std::string("");
        std::map<std::string, std::string>::iterator it = _querey.begin();
        std::cout << "querey : " << it->first << " : " << it->second << "\n";
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

void HttpRequest::checkRequestStartLine(std::string const &startLine)
{
    // check 3 parameters of starline exist
    std::cout << "startline : {" << startLine << "}";
    if (startLine.length() != (_method.length() + _httpVersion.length() + _path.length() + 2)) // +2 for the spaces between parameters 
        std::cout << "{{{{{{{{{{{{{{{{{{{{{{{{{{}}}}}}}}}}}}}}";
    if (_method != "GET" && _method != "DELETE" && _method != "POST")
        _requestStatus = 501; // 501 Not Implemented
    if (_httpVersion != "HTTP/1.1" && _httpVersion != "HTTP/1.1")
        _requestStatus = 505; // 505 HTTP Version Not Supported
}

void HttpRequest::checkRequestkHeaders(std::string const &headerKey)
{
    // check if Host header  exist or duplicated LOL
    if (headerKey == "Host")
        if (_headers.find("Host") != _headers.end())
            _requestStatus = 400; // bad request
    if (headerKey == "host")
        if (_headers.find("host") != _headers.end())
            _requestStatus = 400;
}

/*
 *   Request Body
 */

void HttpRequest::parseRequestBody()
{
    _requestBody = _request.c_str() + _requestIndex; // skip + 4 : /r/n/r/n
    std::cout << "\n-------------------------  BODY BEGIN -------------------------\n";
    std::cout << _requestBody << "\n";
    std::cout << "\n-------------------------  BODY END -------------------------\n";

    _bodyExist = _requestBody == "\r\n" ? false : true;
    if (_method == "POST")
    {
        if (_headers.find("Content-Length") == _headers.end()) // content-type not exist
            _requestStatus = 400;
        if (_bodyExist)
        {
            if (_headers.find("Content-Type") == _headers.end())
                _bodyDataType = "undifined";
            else
            {
                if (_headers["Content-Type"].find("multipart/form-data") != std::string::npos)
                {
                    _bodyDataType = "multipart/form-data";
                    _boundary = _headers["Content-Type"].substr(_headers["Content-Type"].find("=") + 1);
                    std::cout << "Boundery : " << _boundary << "\n";
                    std::string beginBoundary = "--" + _boundary + "\r\n";

                    int found = 0, count = 0;
                    
                    // count how many dataform sent
                    while (_requestBody.find(beginBoundary, found) != std::string::npos)
                    {
                        found = _requestBody.find(beginBoundary, found) + beginBoundary.length();
                        count++;
                    }
                    int i = 0;
                    found = 0;
                    while (i < count)
                    {
                        
                    }
                    
                    std::cout << "CCCCCCC       : "  << count << "\n";
                }
                else
                {
                    _bodyDataType = _headers["Content-Type"];
                }
            }
        }
    }
}

/*
**  request
*/
int HttpRequest::getRequestStatus() const
{
    return _requestStatus;
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