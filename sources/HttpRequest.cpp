
#include "../headers/HttpRequest.hpp"

HttpRequest::HttpRequest(std::string const &request)
    : _requestIndex(0), _request(request), _requestStatus(OK), _bodyExist(false)
{
}

void HttpRequest::initRequest()
{
    this->parseStartLine();
    if (_requestStatus != OK)
        return;
    this->parseHeaders();
    if (_requestStatus != OK)
        return;
    if (_headers.find("Connection") != _headers.end())
        _connectionType = _headers["Connection"];
    else
        _connectionType = "keep-alive";
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
    int httpVersionEndPostion = _request.find(CRLF, pathEndPostion);
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
        _querey = parseQueries(_quereyData);
    }
    _requestIndex = httpVersionEndPostion + 2; // +2 : \r\n end of line
}
std::map<std::string, std::string> HttpRequest::parseQueries(std::string const &queries)
{
    int i = 0;
    int queriesStart = 0;
    std::map<std::string, std::string> mapedQueries;

    while (i < static_cast<int>(queries.length()))
    {
        int quereyend;
        // get the endex where query end
        if (queries.find("&", queriesStart) != std::string::npos)
            quereyend = queries.find("&", queriesStart);
        else
            quereyend = static_cast<int>(queries.length());

        if (queries.find("=", queriesStart) != std::string::npos && static_cast<int>(queries.find("=", queriesStart)) < quereyend)
        {
            mapedQueries[queries.substr(queriesStart, queries.find("=", queriesStart) - queriesStart)] =
                queries.substr(queries.find("=", queriesStart) + 1, quereyend - queries.find("=", queriesStart) - 1); // dont
        }
        else
            mapedQueries[queries.substr(queriesStart, quereyend - queriesStart)] = "";
        queriesStart = quereyend + 1;
        i = queriesStart;
    }
    return mapedQueries;
}

void HttpRequest::parseHeaders()
{
    int headerEndIndex = _request.find("\r\n\r\n"); // get the end of headers
    while (_requestIndex < headerEndIndex)
    {
        // if problem in headers syntax stop parse return bad request
        if (_request.find(":", _requestIndex) == std::string::npos)
        {
            _requestStatus = BAD_REQUEST;
            return;
        }

        /// get headers as key, value in map _headers
        int separator = _request.find(":", _requestIndex);
        /// check before and after separator
        if (_request[separator - 1] == ' ' || _request[separator + 1] != ' ')
        {
            _requestStatus = BAD_REQUEST;
            return;
        }
        int endLine = _request.find(CRLF, _requestIndex);
        std::string headerKey = _request.substr(_requestIndex, separator - _requestIndex);
        if (headerKey == "Host" || headerKey == "host" || headerKey == "HOST")
            if (!this->checkHostHeader())
                return;
        _headers[_request.substr(_requestIndex, separator - _requestIndex)] = /* separator - i : to doesn't include separator  */
            _request.substr(separator + 2, endLine - separator - 2);          // separator + 2 : first charcter of value ;endLine - separator - 2 : index of last character of headers-entities

        _requestIndex = endLine + 2; // for \r\n
    }
    /// check is Host header exist if not bad request
    if (!this->checkRequestkHeaders())
        return;
}

bool HttpRequest::checkRequestStartLine(std::string const &startLine)
{
    // check 3 parameters of starline exist
    // if more then 3 o less return bad request
    if (startLine.length() != _method.length() + _path.length() + _httpVersion.length() + 2) // + 2 for two spaces separate paramters
    {
        _requestStatus = BAD_REQUEST;
        return false;
    }
    if (_method.find(" ") != std::string::npos || _path.find(" ") != std::string::npos || _httpVersion.find(" ") != std::string::npos)
    {
        _requestStatus = BAD_REQUEST;
        return false;
    }
    if (_method != "GET" && _method != "DELETE" && _method != "POST")
    {
        _requestStatus = NOT_IMPLLIMENTED;
        return false;
    }
    if (_httpVersion != "HTTP/1.1")
    {
        _requestStatus = HTTP_VERSION_NOT_SUPPORTED; // 505 HTTP Version Not Supported
        return false;
    }
    return (_requestStatus == OK);
}

bool HttpRequest::checkHostHeader()
{
    // check if Host header  exist or duplicated
    if (_headers.find("Host") != _headers.end() || _headers.find("host") != _headers.end() || _headers.find("HOST") != _headers.end())
    {
        _requestStatus = BAD_REQUEST;
        return false;
    }
    return (_requestStatus == OK);
}

bool HttpRequest::checkRequestkHeaders()
{
    if (_headers.find("Host") == _headers.end() && _headers.find("host") == _headers.end() && _headers.find("HOST") == _headers.end()) // no Host header found
    {
        _requestStatus = BAD_REQUEST;
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
        _requestStatus = BAD_REQUEST;
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
        _requestStatus = BAD_REQUEST;
        return false;
    }
    return (_requestStatus == OK);
}

/*
 *   Request Body
 */

void HttpRequest::parseRequestBody()
{
    //_requestBody = _request.c_str() + _requestIndex; // skip + 4 : /r/n/r/n
    _requestBody = _request.substr(_requestIndex);
    _bodyExist = true;
    if (_headers.find("Content-Length") == _headers.end()) // content-type not exist
    {
        _requestStatus = BAD_REQUEST;
        return;
    }
    if (_bodyExist)
    {
        if (_headers.find("Content-Type") == _headers.end())
        {
            _requestStatus = BAD_REQUEST;
            return;
        }
        else
        {
            _bodyDataType = _headers["Content-Type"];
            if (_headers["Content-Type"].find("multipart/form-data") != std::string::npos)
            {
                this->parseDataFormat();
            }
            else if (_headers["Content-Type"].find("x-www-form-urlencoded") != std::string::npos)
            {
                this->parseformUrlencoded();
            }
            else
            {
                t_bodyPart data;
                data._contType = _bodyDataType;
                data._data = _requestBody.substr(2); // skip \r\n
                _bodyParts.push_back(data);
            }
        }
    }
}

void HttpRequest::parseDataFormat()
{
    // get boundery begin boundary and end boundary
    _bodyDataType = "multipart/form-data";
    _boundary = _headers["Content-Type"].substr(_headers["Content-Type"].find("=") + 1);
    std::string beginBoundary = "--" + _boundary + CRLF;
    std::string endBoundary = "--" + _boundary + "--";

    int begin = 0, end = 0;
    int beginData = 0, endData = 0;
    std::string rawData;
    while (_requestBody.find(beginBoundary, beginData) != std::string::npos)
    {
        t_bodyPart bodyPart;
        // get rawData betwwen boundaries
        beginData = _requestBody.find(beginBoundary, beginData) + beginBoundary.length();
        if (_requestBody.find(beginBoundary, beginData) != std::string::npos)
            endData = _requestBody.find(beginBoundary, beginData) - 2; // \r\n
        else
            endData = _requestBody.find(endBoundary, beginData) - 2;
        rawData = _requestBody.substr(beginData, endData - beginData);

        // get body part form rawData
        begin = rawData.find(":") + 2;
        end = rawData.find(";");
        bodyPart._conDisposition = rawData.substr(begin, end - begin);
        begin = rawData.find("=", end) + 2; // 2: ="
        if (rawData.find("filename") == std::string::npos)
        {
            end = rawData.find(CRLF) - 1; // 1 : "
            bodyPart._name = rawData.substr(begin, end - begin);
            bodyPart._filename = ""; // not file exist
        }
        else
        {
            end = rawData.find(";", end + 1) - 1; // -1: "
            bodyPart._name = rawData.substr(begin, end - begin);
            begin = rawData.find("filename=") + static_cast<int>(strlen("filename=\""));
            end = rawData.find(CRLF) - 1; // 1 : "
            bodyPart._filename = rawData.substr(begin, end - begin);
        }
        // get content:type
        if (rawData.find("Content-Type") == std::string::npos)
            bodyPart._contType = "";
        else
        {
            begin = rawData.find("Content-Type") + static_cast<int>(strlen("Content-Type:\""));
            end = rawData.find(CRLF, begin);
            bodyPart._contType = rawData.substr(begin, end - begin);
        }
        // get data
        bodyPart._data = rawData.substr(rawData.find("\r\n\r\n") + 4);
        _bodyParts.push_back(bodyPart);
    }
}

void HttpRequest::parseformUrlencoded()
{
    std::map<std::string, std::string> data = parseQueries(_requestBody);
    for (std::map<std::string, std::string>::iterator it = data.begin(); it != data.end(); it++)
    {
        t_bodyPart formUrlencoded;
        formUrlencoded._name = it->first;
        formUrlencoded._data = it->second;
        formUrlencoded._contType = "x-www-form-urlencoded";
        formUrlencoded._conDisposition = "";
        formUrlencoded._filename = "";
        _bodyParts.push_back(formUrlencoded);
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

    std::cout << "\n==================  REQUEST BEGIN ==================\n";
    std::cout << _request;
    std::cout << "\n==================  REQUEST END   ==================\n";

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
