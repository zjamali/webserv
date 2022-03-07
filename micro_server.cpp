#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read
#include <vector>
#include <string>

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
    sockaddr.sin_port = htons(9995); // htons is necessary to convert a number to
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
        char buffer[1000];
        int bytesRead = read(connection, buffer, 1000);
        (void)bytesRead;
        std::string str(buffer);
        std::cout << "\n  lenght  " << str.length() << "\n";
        std::cout << buffer;

        std::vector<std::string> requestLines;

        int index = 0;
        int startIndex = 0;
        while (index < (int)str.length())
        {

            while (index < (int)str.length())
            {
                if (str[index] == '\r' && str[index + 1] == '\n')
                {
                    break;
                }
                //if (str[index] == '\r')
                //    std::cout << "-----------------\n";
                index = index + 2;
            }
            std::string str1 = str.substr(startIndex,index - 1);
            std::cout << "{ " << str1 << " }\n"; 
            requestLines.push_back(str1);
            index= index + 2;
            startIndex = index;
        }
        std::cout << " vector" <<requestLines.size() << "\n";

        std::cout << "\\\\\\\n";
        for (std::vector<std::string>::iterator it = requestLines.begin(); it != requestLines.end(); it++)
        {
            std::cout << *it << std::endl;
        }
        std::cout << "**************************\n";
        std::string response = "Good talking to you\n";
        send(connection, response.c_str(), response.size(), 0);
        // close(connection);
    }
    close(sockfd);
    // Read from the connection

    // Send a message to the connection
    // Close the connections
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