#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP
#include <iostream>
#include <string>
#include <map>

class HttpRequest
{
private:
    int _requestIndex;
    std::string _request;
    std::string _method;
    std::string _httpVersion;
    std::string _path;

    std::map<std::string,std::string> _headers;

    void parseStartLine();
    void parseHeaders();
public:
    HttpRequest(std::string const &request);
    ~HttpRequest();
    void print() const;
};
#endif