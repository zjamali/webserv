#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <dirent.h>
#include <time.h>
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

    std::map<unsigned int, std::string> __codes;
    std::map<std::string, std::string> __contentTypesList;
    std::map<unsigned int, std::string> __errorPages;

private:
    //HttpRequest const &_request;
    unsigned int _responseStatus;
    std::string _finaleResponse;


    std::string _method;
    std::string _path;
    std::string _host;
    std::string _httpVersion;
    std::vector<t_bodyPart> _postRequestData;

    void init_response();
    std::string getLocalTime() const;
    std::string generateStartLine(unsigned int const &status_code);
    std::string generateHeader(unsigned int const &status_code, unsigned int const &body_lenght, std::string const &content_type);
    std::string generateBody();

private:
    std::string ResponseOK() const;
    
    bool _errorPagesExist;
    bool _autoIndex;
    std::string const defaultServerPages(unsigned int &statusCode) const;
    std::string const generateErrorResponse(unsigned int errorCode);
    std::string const ResponseBadRequest() const;
    std::string const ResponseForbidden() const;
    std::string const ResponseNotFound() const;
    std::string const ResponseMethodNotAllowed() const;
    std::string const ResponseHttpVersionNotSupported() const;

public:
    HttpResponse(HttpRequest const &request);
    ~HttpResponse();

    std::string generateResponse(unsigned int const code_status, std::string const &root /*or location*/, std::string const &path, std::string const &uploadPath);
    std::string const &getResponse() const { return _finaleResponse;};
    std::string handle_GET_Request(std::string const &root,std::string const &path);
    std::string handleRedirection(std::string const &host, std::string const &location);
    std::string handle_POST_Request(std::string const &root,std::string const &path);
    std::string handle_DELETE_Request(std::string const &root, std::string const &path);
    

    void print();
    std::string readFile(std::string const &file_path);
    std::string readDirectory(std::string const &root, std::string const &path);
    bool upload(std::string const &path, std::string const &filename, std::string const &data);

    // cgi 
    std::string CGI_GET_Request(std::string const &root, std::string const &path);
    std::string run_CGI(std::string const &filename);
};

#endif