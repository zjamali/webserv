#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <sys/stat.h>
#include "HttpRequest.hpp"

class HttpResponse
{
private:
    std::string CRLF_Combination;
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
    std::string __contentType;
    std::string __Date;
    std::string __server;

    std::map<int, std::string> __codes;

    std::map<int, std::string> __errorPages;

private:
    //HttpRequest const &_request;
    std::string _finaleResponse;
    int _responseStatus;
    std::string _method;
    std::string _path;
    std::string _httpVersion;
    std::map<std::string, std::string> _responseHeaders;

    std::string _responseBody;

    void init_response();
    std::string getLocalTime() const;
    std::string getStartLine() const;
    std::string getHeaders() const;
    std::string getBody();

private:
    std::string ResponseOK() const;
    std::string const ResponseBadRequest() const;
    std::string const ResponseNotFound() const;
    std::string const ResponseMethodNotAllowed() const;
    std::string const ResponseHttpVersionNotSupported() const;

public:
    HttpResponse(HttpRequest const &request);
    ~HttpResponse();

    std::string generateResponse();
    std::string const &getResponse() const { return _finaleResponse;};
    void print();
    friend bool createFile(std::string const &path, std::string const &filename, std::string const &data);
};

#endif