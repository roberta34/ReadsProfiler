#pragma once

#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unordered_map>

#include "Protocol.hpp"
#include "RequestParser.hpp"

using namespace std;

class ConnectionManager{
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen;
    unordered_map<int, string> socketToUser;
    public: 
        ConnectionManager();
        void run();
};