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
        //bytesRead = read(connection, buffer, 10000);
        std::string recievedData;
        while ((bytesRead = recv(connection, buffer, 10000, 0)) > 0)
        {
             buffer[bytesRead] = '\0';
            std::cout << "-----------------------------readed " << bytesRead  << "\n";
            std::cout << buffer << std::endl;
           
            recievedData = recievedData + buffer;
            if (bytesRead < 10000)
                break;
        }

        (void)bytesRead;
        std::cout << "zabi       --------\n";
        std::cout << recievedData;
         std::cout << "-------------\n";
        /////// request parse begin 
        HttpRequest request(recievedData);
        ////// request parse end
        request.print();

        HttpResponse response(request);
        /*
        std::ifstream fs;
        fs.open("./serverPages/200.html");

        char *indexData = (char*)malloc(2000);
        fs.read(indexData,2000);
        fs.close();
        std::string header("HTTP/1.1 200 OK\r\nVary: Origin\r\nAccess-Control-Allow-Credentials: true\r\nAccept-Ranges: bytes\r\nCache-Control: public, max-age=0\r\nLast-Modified: Wed, 09 Mar 2022 18:28:40 GMT\r\nDate: Wed, 09 Mar 2022 18:43:38 GMT\r\nConnection: keep-alive\r\nKeep-Alive: timeout=5\r\n\r");
        // std::string response1 = std::string("HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer:  webServ\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 20000\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r") + std::string(indexData);
        std::string response1 = header + std::string(indexData);
        //std::string myResponse = "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer:  webServ\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 88\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\n";
        // send(connection, myResponse.c_str(), myResponse.size(), 0);

        
        free(indexData);*/
        response.print();
        // send(connection, response1.c_str(), response1.size(), 0);
        send(connection, response.generateResponse().c_str(), response.generateResponse().length() , 0);
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