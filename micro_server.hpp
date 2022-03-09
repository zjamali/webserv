#ifndef MICRO_SERVER_HPP
#define MICRO_SERVER_HPP

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "HttpRequest.hpp"

#define PORT 9997

#endif