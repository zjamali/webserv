#include "HttpRequest.hpp"

HttpRequest::HttpRequest(std::string const &request)
    : _requestIndex(0), _request(request), _requestStatus(0), _bodyExist(false)
{
    this->parseStartLine();
    if (_requestStatus != 0)
        return;
    this->parseHeaders();
    if (_requestStatus != 0)
        return;

    if (_method == "POST")
        this->parseRequestBody();
    if (_requestStatus != 0)
        return;
}
HttpRequest::~HttpRequest()
{
}

/*
 *   Request header
 */
void HttpRequest::parseStartLine()
{
    // get start line:
    std::string startLine = _request.substr(0, _request.find("\r\n"));

    // get method
    int methodEndPostion = _request.find(" ");
    _method = _request.substr(0, methodEndPostion);

    // get full path with queries
    methodEndPostion++;
    int pathEndPostion = _request.find(" ", methodEndPostion);
    _path = _request.substr(methodEndPostion, pathEndPostion - methodEndPostion);

    // get http vertion
    pathEndPostion++;
    int httpVersionEndPostion = _request.find("\r\n", pathEndPostion);
    _httpVersion = _request.substr(pathEndPostion, httpVersionEndPostion - pathEndPostion);

    // check start line parameters
    if (!this->checkRequestStartLine(startLine)) // stop parse if start line not correct
        return;

    // get quesris if exist
    if (_path.find("?") != std::string::npos)
    {
        /// get queries
        // and change path to correct path
        int pathEnd = _path.find("?");
        _quereyData = _path.substr(pathEnd + 1);

        // get correct path without queries
        std::string newPath = _path.substr(0, pathEnd);
        _path = newPath;

        // get queries
        int i = 0;
        int queriesStart = 0;
        while (i < static_cast<int>(_quereyData.length()))
        {
            int quereyend;
            // get the endex where query end
            if (_quereyData.find("&", queriesStart) != std::string::npos)
                quereyend = _quereyData.find("&", queriesStart);
            else
                quereyend = static_cast<int>(_quereyData.length());

            if (_quereyData.find("=", queriesStart) != std::string::npos 
                && static_cast<int>(_quereyData.find("=", queriesStart)) < quereyend)
            {
                _querey[_quereyData.substr(queriesStart, _quereyData.find("=", queriesStart) - queriesStart)] =
                    _quereyData.substr(_quereyData.find("=", queriesStart) + 1, quereyend - _quereyData.find("=", queriesStart) - 1); // dont
            }
            else
                _querey[_quereyData.substr(queriesStart, quereyend - queriesStart)] = "";
            queriesStart = quereyend + 1;
            i = queriesStart;
        }
    }
    std::cout << "\n================================================================\n";
    for (std::map<std::string, std::string>::iterator it = _querey.begin(); it != _querey.end(); it++)
    {
        std::cout << it->first << " : " << it->second << std::endl;
    }

    std::cout << "\n================================================================\n";
    _requestIndex = httpVersionEndPostion + 2; // +2 : \r\n end of line
}

void HttpRequest::parseHeaders()
{
    int headerEndIndex = _request.find("\r\n\r\n"); // get the end of headers
    while (_requestIndex < headerEndIndex)
    {
        // if problem in headers syntax stop parse return bad request
        if (_request.find(":", _requestIndex) == std::string::npos)
        {
            _requestStatus = 400;
            return;
        }

        /// get headers as key, value in map _headers
        int separator = _request.find(":", _requestIndex);
        /// check before and after separator
        if (_request[separator - 1] == ' ' || _request[separator + 1] != ' ')
        {
            _requestStatus = 400;
            return;
        }
        int endLine = _request.find("\r\n", _requestIndex);
        std::string headerKey = _request.substr(_requestIndex, separator - _requestIndex);
        if (headerKey == "Host" || headerKey == "host" || headerKey == "HOST")
            if (!this->checkHostHeader())
                return;
        _headers[_request.substr(_requestIndex, separator - _requestIndex)] = /* separator - i : to doesn't include separator  */
            _request.substr(separator + 2, endLine - separator - 2);          // separator + 2 : first charcter of value ;endLine - separator - 2 : index of last character of headers-entities

        _requestIndex = endLine + 2; // for \r\n
    }
    if (!this->checkRequestkHeaders())
        return;
    /// check is Host header exist if not bad request
}

bool HttpRequest::checkRequestStartLine(std::string const &startLine)
{
    // check 3 parameters of starline exist
    // if more then 3 o less return bad request
    std::cout << startLine << std::endl;
    if (startLine.length() != _method.length() + _path.length() + _httpVersion.length() + 2) // + 2 for two spaces separate paramters
    {
        _requestStatus = 400;
        return false;
    }
    if (_method.find(" ") != std::string::npos || _path.find(" ") != std::string::npos || _httpVersion.find(" ") != std::string::npos)
    {
        _requestStatus = 400;
        return false;
    }
    if (_method != "GET" && _method != "DELETE" && _method != "POST")
    {
        _requestStatus = 501; // 501 Not Implemented
        return false;
    }
    if (_httpVersion != "HTTP/1.1" && _httpVersion != "HTTP/1.1")
    {
        _requestStatus = 505; // 505 HTTP Version Not Supported
        return false;
    }
    return (_requestStatus == 0);
}

bool HttpRequest::checkHostHeader()
{
    // check if Host header  exist or duplicated
    if (_headers.find("Host") != _headers.end() || _headers.find("host") != _headers.end() || _headers.find("HOST") != _headers.end())
    {
        _requestStatus = 400;
        return false;
    }
    return (_requestStatus == 0);
}

bool HttpRequest::checkRequestkHeaders()
{
    if (_headers.find("Host") == _headers.end() && _headers.find("host") == _headers.end() && _headers.find("HOST") == _headers.end()) // no Host header found
    {
        _requestStatus = 400;
        return false;
    }
    std::string hosts[3] = {"Host", "host", "HOST"};
    int i = 0;
    for (; i < 3; i++)
    {
        if (_headers.find(hosts[i]) != _headers.end())
            break;
    }

    if (_headers[hosts[i]].empty())
    {
        _requestStatus = 400;
        return false;
    }
    std::string host = _headers[hosts[i]];
    i = 0;
    // check if the host only containe withspaces
    for (; i < static_cast<int>(host.length()); i++)
    {
        if (!isspace(host[i]))
            break;
    }
    if (i == static_cast<int>(host.length()))
    {
        _requestStatus = 400;
        return false;
    }

    return (_requestStatus == 0);
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

                    std::cout << "CCCCCCC       : " << count << "\n";
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