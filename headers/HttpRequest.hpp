#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <fstream>
#include <ctype.h>

#define CRLF "\r\n"
/// 
#define OK 200
#define CREATED 201
#define ACCEPTED 202
#define NO_CONTENT 204
#define MOVED_PERMANENTLY 301
#define FOUND 302
#define TEMPORARY_REDIRECT 307
#define PERMANENT_REDIRECT 308
#define BAD_REQUEST 400
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define INTERNAL_SERVER_ERROR 500 
#define NOT_IMPLLIMENTED 501
#define HTTP_VERSION_NOT_SUPPORTED 505

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
    void initRequest();
    int getRequestStatus() const;
    void print() const;

    /*
     *   Request header
     */
private:
    int _port;
    std::string _host;
    std::string _request;
    std::string _method;
    std::string _httpVersion;
    std::string _path;
    std::string _connectionType;
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
    void setBuffer(std::string const &buff){_request = buff;};
    void setPort(int const &port){_port=port;};
    void setHost(std::string const &host){_host=host;};

    std::string getConnectionType() const{ return _connectionType;};
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
    std::string getRequestBody() const { return _requestBody;};
    
    std::vector<t_bodyPart> getBodyParts() const {return _bodyParts;};

public:
    HttpRequest():_requestIndex(0), _request(""), _requestStatus(OK), _bodyExist(false)
    {};
    HttpRequest(std::string const &request);
    ~HttpRequest();
};
#endif