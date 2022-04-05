#include "micro_server.hpp"

int main()
{

    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(PORT); // htons is necessary to convert a number to
                                     // network byte order
    if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start listening. Hold at most 10 connections in the queue
    if (listen(sockfd, 10) < 0)
    {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Grab a connection from the queue
    size_t addrlen = sizeof(sockaddr);

    while (1)
    {
        /* code */

        int connection = accept(sockfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
        if (connection < 0)
        {
            std::cout << "Failed to grab connection. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }
        char buffer[10000];
        int bytesRead;
        // bytesRead = read(connection, buffer, 10000);
        std::string recievedData;
        unsigned int request_lenght = 900000000;
        int i = 1;
        while ((bytesRead = recv(connection, buffer, 10000, 0)) > 0)
        {
            buffer[bytesRead] = '\0';
            recievedData += buffer;
            // read the all request
            if (recievedData.find("POST") != std::string::npos)
            {
                // read the all request
                if (i && recievedData.find("Content-Length:") != std::string::npos)
                {
                    i = 0;
                    int begin = recievedData.find("Content-Length:") + strlen("Content-Length: ");
                    int end = recievedData.find("\r\n", begin);
                    std::string contentLength = recievedData.substr(begin, end - begin);
                    std::string header = recievedData.substr(0, recievedData.find("\r\n\r\n"));
                    request_lenght = (header.length()) + atoi(contentLength.c_str()) + strlen("\r\n\r\n");
                }
                if (request_lenght < 900000000)
                {
                    if (recievedData.length() >= request_lenght)
                        break;
                }
            }
            else
            {
                if (recievedData.find("\r\n\r\n") != std::string::npos)
                    break;
            }
        }

        (void)bytesRead;
        /////// request parse begin
        HttpRequest request(recievedData);
        request.initRequest();
        request.print();
        ////// request parse end
        HttpResponse response(request);
       // response.print();
        //response.print();
        send(connection, response.getResponse().c_str(), response.getResponse().length(), 0);
        close(connection);
    }
    close(sockfd);
}

/*
parsing request, set status code
parse first header -> method: [GET, POST, DELETE], url(3la 7ssab config file), protocol = HTTP/1.1
everythings else: map[key] = value

example for post method:
special variable: std::string postbody
POST / HTTP/1.1
Host: localhost:9998
Content-Type: application/json
Content-Length: 15

{
    "t": 5,
    "a": 6
}

example for post method with files:
struct fileParse {
    std:string ConDisposition;
    std:string ContType;
    std::string body;
    string filename;
}
special variable for files: std::vector<struct fileParse> v;
POST / HTTP/1.1
Host: localhost:9998
other-headers
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryePkpFF7tjBAqx29L

------WebKitFormBoundaryePkpFF7tjBAqx29L
Content-Disposition: form-data; name="MAX_FILE_SIZE"

100000
------WebKitFormBoundaryePkpFF7tjBAqx29L
Content-Disposition: form-data; name="uploadedfile"; filename="hello.o"
Content-Type: application/x-object

... contents of file goes here ...
------WebKitFormBoundaryePkpFF7tjBAqx29L--
*/