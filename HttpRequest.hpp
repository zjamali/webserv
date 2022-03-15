#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP
#include <iostream>
#include <string>
#include <map>
#include <utility>

class HttpRequest
{
private:
    int _requestIndex;
    std::string _request;
    std::string _method;
    std::string _httpVersion;
    std::string _path;
    int _requestStatus;
    std::string _queriesData;
    std::map<std::string, std::string> _queries;
    std::map<std::string, std::string> _headers;

    bool _bodyExist;
    std::string _requestBody;

    void parseStartLine();
    void parseHeaders();
    void checkRequestStartLine();
    void checkRequestkHeaders(std::string const  &headerKey);
    void parseRequestBody();
    void parseBodyparts();

public:
    HttpRequest(std::string const &request);
    ~HttpRequest();
    std::string getMethod() const;
    std::string getHttpVersion() const;
    std::string getPath() const;
    int getRequestStatus() const; 
    std::map<std::string, std::string> getHedaers() const;
    std::map<std::string, std::string> getQueries() const;
    void print() const;
};
#endif