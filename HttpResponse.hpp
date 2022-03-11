#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include <iostream>
#include <string>
#include <map>
#include <utility>

class HttpResponse
{
private:
    std::string _generatedResponse;
    std::string _responseStatus;
    std::string _method;
    std::string _httpVersion;
    std::map<std::string, std::string> _responseHeaders;

    std::string _responseBody;
    
public:
    HttpResponse(/* args */);
    ~HttpResponse();
};

HttpResponse::HttpResponse(/* args */)
{
}

HttpResponse::~HttpResponse()
{
}

#endif