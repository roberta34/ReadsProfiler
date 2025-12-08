#pragma once

#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "Protocol.hpp"
#include "RequestParser.hpp"

class ConnectionManager{
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen;
    public: 
        ConnectionManager();
        void run();
};