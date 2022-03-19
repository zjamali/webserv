#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <ctype.h>

class bodyPart
{
private:
    std::string _conDisposition;
    std::string _contType;
    std::string _name;
    std::string _data;
    std::string _filename;

public:
    bodyPart(){};
    bodyPart(std::string const conDispo, std::string const conType,std::string const data, std::string const filename):
    _conDisposition(conDispo), _contType(conType), _data(data), _filename(filename) {}
    ~bodyPart(){};

    std::string getContentDispostion() const { return _conDisposition;};
    std::string getContentType() const { return _contType;};
    std::string getData() const { return _data;};
    std::string getFilename() const { return _filename;};

};

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
    std::vector<bodyPart> _bodyParts;
    void parseRequestBody();
    void parseBodyparts();


public:
    std::string getRequestBody() const;
    
    std::vector<bodyPart> getBodyParts() const;

public:
    HttpRequest(std::string const &request);
    ~HttpRequest();
};
#endif