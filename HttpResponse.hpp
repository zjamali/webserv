#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include "HttpRequest.hpp"






class HttpResponse
{
private:
    /*
    *   RESPONSE HEDAER
    */
    //// start line
    std::string __http;
    std::string __statusCode;
    std::string __statusDesciption;
    //// hedaers
    std::string __connection;
    std::string __contentLength;
    std::string __contentType ;
    std::string __Date;
    std::string __server ;

    std::map<int, std::string> __codes;
private:
    HttpRequest const &_request;
    std::string _generatedResponse;
    int _responseStatus;
    std::string _method;
    std::string _path;
    std::string _httpVersion;
    std::map<std::string, std::string> _responseHeaders;

    std::string _responseBody;
    
    std::string getLocalTime();
    std::string getStartLine();
    std::string getHeaders();
    std::string getBody();
public:
    HttpResponse(HttpRequest const &request);
    ~HttpResponse();

    std::string generateResponse();
    void print();
};


#endif