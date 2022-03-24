#include "HttpResponse.hpp"

HttpResponse::HttpResponse(HttpRequest const &request)
{
    // init servers
    init_response();
    _responseStatus = request.getRequestStatus();
    _method = request.getMethod();
    _httpVersion = request.getHttpVersion();
    _path = request.getPath();
    
        _postRequestData = request.getBodyParts();
    std::map<std::string, std::string> _headers = request.getHedaers();
    if (_headers.find("Content-Type:") != _headers.end())
        __contentType = request.getHedaers()["Content-Type:"];
    // generate response
    _errorPagesExist = true;
    __errorPages[NOT_FOUND] = "/Users/zjamali/Desktop/webserv/www/404.html";
    _finaleResponse = generateResponse("/Users/zjamali/Desktop/webserv/www", "/Users/zjamali/Desktop/webserv/www/upload/"); // get config obj ; root
}

void HttpResponse::init_response()
{
    _errorPagesExist = false;
    CRLF_Combination = std::string(CRLF);
    __http = "";
    __statusCode = "";
    __statusDesciption = "";
    __connection = "Closed";
    __contentLength = "150";
    __contentType = "text/html";
    __Date = "";
    __server = "Webserv/1.0";

    //// Successful responses
    __codes[200] = "OK";
    //// Redirection messages
    __codes[300] = "Multiple Choice";
    __codes[301] = "Moved Permanently";
    __codes[307] = "Temporary Redirect";
    __codes[308] = "Permanent Redirect";
    //// Client error responses
    __codes[400] = "Bad Request";
    __codes[404] = "Not Found";
    __codes[405] = "Method Not Allowed";
    //// Server error responses
    __codes[500] = "Internal Server Error";
    __codes[501] = "Not Implemented";
    __codes[505] = "HTTP Version Not Supported";

    ////// conten Types list

    __contentTypesList[".aac"] = "audio/aac";
    __contentTypesList[".abw"] = "application/x-abiword";
    __contentTypesList[".arc"] = "application/x-freearc";
    __contentTypesList[".avif"] = "image/avif";
    __contentTypesList[".avi"] = "video/x-msvideo";
    __contentTypesList[".azw"] = "application/vnd.amazon.ebook";
    __contentTypesList[".bin"] = "application/octet-stream";
    __contentTypesList[".bmp"] = "image/bmp";
    __contentTypesList[".bz"] = "application/x-bzip";
    __contentTypesList[".bz2"] = "application/x-bzip2";
    __contentTypesList[".cda"] = "application/x-cdf";
    __contentTypesList[".csh"] = "application/x-csh";
    __contentTypesList[".css"] = "text/css";
    __contentTypesList[".csv"] = "text/csv";
    __contentTypesList[".doc"] = "application/msword";
    __contentTypesList[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    __contentTypesList[".eot"] = "application/vnd.ms-fontobject";
    __contentTypesList[".epub"] = "application/epub+zip";
    __contentTypesList[".gz"] = "application/gzip";
    __contentTypesList[".gif"] = "image/gif";
    __contentTypesList[".htm"] = "text/html";
    __contentTypesList[".html"] = "text/html";
    __contentTypesList[".ico"] = "image/vnd.microsoft.icon";
    __contentTypesList[".ics"] = "text/calendar";
    __contentTypesList[".jar"] = "application/java-archive";
    __contentTypesList[".jpeg"] = "image/jpeg";
    __contentTypesList[".jpg"] = "image/jpeg";
    __contentTypesList[".js"] = "text/javascript ";
    __contentTypesList[".json"] = "application/json";
    __contentTypesList[".jsonld"] = "application/ld+json";
    __contentTypesList[".mid"] = "audio/midi audio/x-midi";
    __contentTypesList[".mjs"] = "text/javascript";
    __contentTypesList[".mp3"] = "audio/mpeg";
    __contentTypesList[".mp4"] = "video/mp4";
    __contentTypesList[".mpeg"] = "video/mpeg";
    __contentTypesList[".mpkg"] = "application/vnd.apple.installer+xml";
    __contentTypesList[".odp"] = "application/vnd.oasis.opendocument.presentation";
    __contentTypesList[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    __contentTypesList[".odt"] = "application/vnd.oasis.opendocument.text";
    __contentTypesList[".oga"] = "audio/ogg";
    __contentTypesList[".ogv"] = "video/ogg";
    __contentTypesList[".ogx"] = "OGG	a/ogg";
    __contentTypesList[".opus"] = "audio/opus";
    __contentTypesList[".otf"] = "font/otf";
    __contentTypesList[".png"] = "image/png";
    __contentTypesList[".pdf"] = "application/pdf";
    __contentTypesList[".php"] = "application/x-httpd-php";
    __contentTypesList[".ppt"] = "application/vnd.ms-powerpoint";
    __contentTypesList[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    __contentTypesList[".rar"] = "application/vnd.rar";
    __contentTypesList[".rtf"] = "application/rtf";
    __contentTypesList[".sh"] = "application/x-sh";
    __contentTypesList[".svg"] = "image/svg+xml";
    __contentTypesList[".swf"] = "application/x-shockwave-flash";
    __contentTypesList[".tar"] = "application/x-tar";
    __contentTypesList[".tif"] = "image/tiff";
    __contentTypesList[".tiff"] = "image/tiff";
    __contentTypesList[".ts"] = "video/mp2t";
    __contentTypesList[".ttf"] = "font/ttf";
    __contentTypesList[".txt"] = "text/plain";
    __contentTypesList[".vsd"] = "application/vnd.visio";
    __contentTypesList[".wav"] = "audio/wav";
    __contentTypesList[".weba"] = "audio/webm";
    __contentTypesList[".webm"] = "video/webm";
    __contentTypesList[".webp"] = "image/webp";
    __contentTypesList[".woff"] = "font/woff";
    __contentTypesList[".woff2"] = "font/woff2";
    __contentTypesList[".xhtml"] = "application/xhtml+xml";
    __contentTypesList[".xls"] = "application/vnd.ms-excel";
    __contentTypesList[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    __contentTypesList[".xml"] = "application/xml";
    __contentTypesList[".xul"] = "application/vnd.mozilla.xul+xml";
    __contentTypesList[".zip"] = "application/zip";
    __contentTypesList[".3gp"] = "audio/video";
    __contentTypesList[".3g2"] = "audio/video";
    __contentTypesList[".7z"] = "application/x-7z-compressed";
}

HttpResponse::~HttpResponse()
{
}

std::string readFile(std::string const &file_path)
{

    std::ifstream file(file_path);
    if (file)
    {
        std::ostringstream stream_string;
        stream_string << file.rdbuf();
        return stream_string.str();
    }
    else
        return "error file not opened";
}

std::string HttpResponse::getLocalTime() const
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    memset(buffer, '\0', 80);
    strftime(buffer, 80, "%a, %d %h %Y %T GMT", timeinfo);
    return (std::string(buffer));
}

std::string HttpResponse::generateStartLine(unsigned int status_code)
{
    if (__codes.find(status_code) != __codes.end())
        return ("HTTP/1.1 " + std::to_string(status_code) + __codes[status_code]);
    else
        return ("HTTP/1.1 " + std::to_string(NOT_FOUND) + __codes[NOT_FOUND]);
}

std::string HttpResponse::generateHeader(unsigned int const status_code, unsigned int body_lenght, std::string const content_type)
{
    std::string header;
    header += generateStartLine(status_code) + CRLF_Combination;
    header += "Content-Type: " + content_type + CRLF_Combination;
    header += "Content-Lenght: " + std::to_string(body_lenght) + CRLF_Combination;
    header += "Server: " + __server;
    return header;
}

void HttpResponse::print()
{
    std::cout << "+++++++++++++++++++++ Responce BEGIN ++++++++++++++++++++\n";
    std::cout << _finaleResponse << std::endl;
    std::cout << "+++++++++++++++++++++ Responce END ++++++++++++++++++++\n";
}

std::string HttpResponse::generateBody()
{
    return " ";
}

std::string const HttpResponse::defaultServerPages(unsigned int statusCode) const
{
    if (statusCode == OK)
        return ResponseOK();
    else if (statusCode == BAD_REQUEST)
        return ResponseBadRequest();
    else if (statusCode == NOT_FOUND)
        return ResponseNotFound();
    else if (statusCode == METHOD_NOT_ALLOWED)
        return ResponseMethodNotAllowed();
    else if (statusCode == HTTP_VERSION_NOT_SUPPORTED)
        return ResponseHttpVersionNotSupported();
    else if (statusCode == NOT_IMPLLIMENTED)
        return ResponseMethodNotAllowed();
    else
        return ResponseNotFound();
}

std::string const HttpResponse::generateErrorResponse(unsigned int errorCode)
{
    if (_errorPagesExist)
    {
        if (__errorPages.find(errorCode) == __errorPages.end())
            return defaultServerPages(errorCode);
        else
        {
            std::string body;
            // get Html page path
            std::string pagePath = __errorPages[errorCode];
            struct stat sb;
            if (!stat(pagePath.c_str(), &sb) && S_ISREG(sb.st_mode))
            {
                int fd = open(pagePath.c_str(), O_RDONLY);
                if (fd > 0)
                {
                    char buffer[(int)sb.st_size];
                    int readedBytes;
                    while ((readedBytes = read(fd, buffer, sb.st_size)) > 0)
                    {
                        buffer[readedBytes] = '\0';
                        body += buffer;
                        if (body.length() >= (unsigned int)sb.st_size)
                            break;
                    }
                    close(fd);
                }
            }
            return (generateHeader(errorCode, body.length(), __contentType) + CRLF_Combination + CRLF_Combination + body);
        }
    }
    else
    {
        return defaultServerPages(errorCode);
    }
}

std::string HttpResponse::generateResponse(std::string const &root /*or location*/, std::string const &uploadPath)
{
    std::string response;
    std::string header;
    std::string body;

    if (_responseStatus != 200)
    {
        return generateErrorResponse(_responseStatus);
    }
    ///
    if (_method == "GET")
    {
        /// check the path is correct and file request exist
        struct stat sb;
        if (stat(root.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        {
            /// search for index
            if (_path == "/")
            {
                if (stat(std::string(root + "/" + "index.html").c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
                    body = readFile(std::string(root + "/" + "index.html"));
                else
                    return generateErrorResponse((_responseStatus = NOT_FOUND));
            }
            else
            {
                struct stat sb;
                std::string filePath = root + _path;
                if (stat(filePath.c_str(), &sb) == 0 && S_ISREG(sb.st_mode) && access(filePath.c_str(), R_OK) == 0)
                {
                    if (_path.find(".") != std::string::npos)
                    {
                        std::string filetype = _path.substr(_path.find(".", _path.length() - 5));
                        if (__contentTypesList.find(filetype) != __contentTypesList.end())
                            __contentType = __contentTypesList[filetype];
                        body = readFile(filePath);
                    }
                }
                else
                    return generateErrorResponse((_responseStatus = NOT_FOUND));
            }
        }
        else
            return generateErrorResponse((_responseStatus = NOT_FOUND));
        ///
        header += generateHeader(_responseStatus, body.length(), __contentType);
        // add body

        // end the body

        return (header + CRLF_Combination + CRLF_Combination + body);
    }
    else if (_method == "POST")
    {
        // check if upload location exist
        struct stat sb;
        if (stat(uploadPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)/* && access(uploadPath.c_str(), W_OK)*/)
        {
            for (std::vector<t_bodyPart>::iterator it = _postRequestData.begin(); it != _postRequestData.end(); it++)
            {
                std::cout << "POST POST POSTPOSTPOSTPOSTPOSTPOSTPOST\n";
                if (!(it->_filename.empty())) // a filename exist
                {
                    std::ofstream outFile(uploadPath + it->_filename);
                    if (outFile)
                    {
                        outFile << it->_data;
                    }
                    outFile.close();
                }
            }
            return ResponseOK();
        }
        else
            return generateErrorResponse((_responseStatus == NOT_FOUND));
    }
    else if (_method == "DELETE")
    {
        return "DELETE";
    }
    else
        return generateErrorResponse((_responseStatus == NOT_IMPLLIMENTED));
}

std::string HttpResponse::ResponseOK() const
{
    std::string body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Welcome to webserv!</title> \r\n</head> \r\n<body>\r\n<center><h1>Welcome to webserv</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 200 OK" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8\r\nContent-Length: " + std::to_string(body.length()) + "\r\nConnection: Closed\r\nServer: webserv/1.0\r\nDate: " + getLocalTime();

    return (header + CRLF_Combination + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseBadRequest() const
{
    std::string const body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>400 Bad Request</title> \r\n</head> \r\n<body>\r\n<center><h1>400 Bad Request</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 400 Bad Request" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime();
    return (header + CRLF_Combination + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseNotFound() const
{
    std::string const body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>404 Not Found</title> \r\n</head> \r\n<body>\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 404 Not Found" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime();
    return (header + CRLF_Combination + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseMethodNotAllowed() const
{
    std::string const body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>405 Method Not Allowed</title> \r\n</head> \r\n<body>\r\n<center><h1>405 Method Not Allowed</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 405 Method Not Allowed" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime();
    return (header + CRLF_Combination + CRLF_Combination + body);
}
std::string const HttpResponse::ResponseHttpVersionNotSupported() const
{
    std::string const body = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>505 HTTP Version Not Supported</title> \r\n</head> \r\n<body>\r\n<center><h1>505 HTTP Version Not Supported</h1></center>\r\n<hr><center><p>webserv/1.0</p></center>\r\n</body>\r\n</html>\r\n";
    std::string const header = "HTTP/1.1 505 HTTP Version Not Supported" + CRLF_Combination + "Content-Type: text/html; charset=UTF-8" + CRLF_Combination + "Content-Length: " + std::to_string(body.length()) + CRLF_Combination + "Connection: Closed" + CRLF_Combination + "Server: webserv/1.0" + CRLF_Combination + "Date: " + getLocalTime();
    return (header + CRLF_Combination + CRLF_Combination + body);
}

bool upload(std::string const &path, std::string const &filename, std::string const &data)
{
    struct stat stats;

    if (stat(path.c_str(), &stats) == 0 && S_ISDIR(stats.st_mode))
    {
        std::ofstream outfile(path + "/" + filename);
        outfile << data;
        outfile.close();
        return 0;
    }
    else
    {
        std::cout << "file not created" << std::endl;
        return 1;
    }
}