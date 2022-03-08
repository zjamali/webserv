#include "micro_server.hpp"

void checkStartLine(std::map<std::string, std::string> &firstLine)
{
    if (firstLine["method"] != "GET" && firstLine["method"] != "POST" && firstLine["method"] != "DELETE")
        std::cout << "**************** error: bad request**************\n";
    
}


/* parseHeader(char *buffer)
*  --- start line
*   - get request method
*   - path requested
*   - http version
*  
*  
*/
void     parseHeader(std::string const &request)
{
    std::map<std::string, std::string> header;
    //std::string request(buffer);
    // start line
    
    int methodEndPostion = request.find(" ");
    header["method"] = request.substr(0,methodEndPostion);
    methodEndPostion++;
    int pathEndPostion = request.find(" ",methodEndPostion);
    header["path"] = request.substr(methodEndPostion, pathEndPostion - methodEndPostion);
    pathEndPostion++;
    int httpVersionEndPostion = request.find("\r\n", pathEndPostion);
    header["http-version"] = request.substr(pathEndPostion, httpVersionEndPostion - pathEndPostion);
    
    
    int headerEndIndex = request.find("\r\n\r\n");
    checkStartLine(header);
    int i = httpVersionEndPostion + 2; // +2 : \r\n
    while (i < headerEndIndex)
    {
        int separator = request.find(":", i);
        int endLine = request.find("\r\n", i);
        header[request.substr(i ,separator - i)/* separator - i : to doesn't include separator  */ ] 
            = request.substr(separator + 2,endLine - separator - 2); // separator + 2 : first charcter of value ;endLine - separator - 2 : index of last character of headers-entities
        i = endLine + 2; 
    }
    
    for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end(); it++)
    {
        std::cout << "{" <<it->first << "} : {" << it->second << "}\n";
    }
    
}

void parseRequest(std::string const &buffer)
{
    parseHeader(buffer);
    std::cout << "***********\n";
}
