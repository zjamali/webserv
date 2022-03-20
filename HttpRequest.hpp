#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <ctype.h>

typedef struct s_bodyPart
{
    std::string _conDisposition;
    std::string _name;
    std::string _filename;
    std::string _contType;
    std::string _data;
}t_bodyPart;

class HttpRequest
{
    /*
     *   Request
     */
private:
    int _requestIndex;

public:
    int getRequestStatus() const;
    void print() const;

    /*
     *   Request header
     */
private:
    std::string _request;
    std::string _method;
    std::string _httpVersion;
    std::string _path;
    int _requestStatus;
    std::string _quereyData; // raw data
    std::map<std::string, std::string> _querey;
    std::map<std::string, std::string> _headers;
    void parseStartLine();
    void parseHeaders();
    bool checkRequestStartLine(std::string const &startLine);
    bool checkHostHeader();
    std::map<std::string, std::string> parseQueries(std::string const &queries);
    bool checkRequestkHeaders();

public:
    std::string getMethod() const { return _method;};
    std::string getHttpVersion() const { return _httpVersion; };
    std::string getPath() const { return _path; };
    std::map<std::string, std::string> getHedaers() const {return _headers; };
    std::map<std::string, std::string> getQueries() const {return _querey; };

    /*
     *   Request Body
     */
private:
    std::string _requestBody;
    bool _bodyExist;
    
    std::string _bodyDataType;
    std::string _boundary;
    std::vector<t_bodyPart> _bodyParts;
    void parseRequestBody();
    void parseBodyparts();
    void parseDataFormat();
    void parseformUrlencoded();

public:
    std::string getRequestBody() const;
    
    std::vector<t_bodyPart> getBodyParts() const;

public:
    HttpRequest(std::string const &request);
    ~HttpRequest();
};
#endif