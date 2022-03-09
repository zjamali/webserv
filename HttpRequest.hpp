#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP
#include <iostream>
#include <string>
#include <map>

typedef enum e_dataType{
    file,text
}dataType;

class HttpRequest
{
private:
    int _requestIndex;
    std::string _request;
    std::string _method;
    std::string _httpVersion;
    std::string _path;
    int _requestStatus;

    std::string _contentType;
    std::string _boundary;
    std::string _requestBody;
    bool _bodyExist;
    typedef struct s_bodyPart
    {
        int dataType;
        std::string contentDisposition;
        std::string contentType;
        std::string name;
        std::string data;
        s_bodyPart(std::string dispostion, std::string type, std::string name, std::string data):
        contentDisposition(dispostion), contentType(type),name(name),data(data)
         {};
    } bodyPart;

    std::map<std::string, std::string> _headers;
    std::vector<bodyPart> _body();

    void parseStartLine();
    void parseHeaders();
    void checkRequestStartLine();
    void checkRequestkHeaders();
    void parseRequestBody();
    void parseBodyparts();

public:
    HttpRequest(std::string const &request);
    ~HttpRequest();
    void print() const;
};
#endif