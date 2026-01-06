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
    //retea
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen;
    //logica
    SearchEngine* searchEngine;
    DownloadManager* downloadManager;
    RecommendationEngine* recommendationEngine; 
    //management ul sesiunii
    unordered_map<int, string> socketToUser;
    public: 
        ConnectionManager(SearchEngine* se, DownloadManager* dm, RecommendationEngine* re);
        void run();
};