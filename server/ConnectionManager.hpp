#pragma once

#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unordered_map>
#include <sqlite3.h>

#include "Protocol.hpp"
#include "RequestParser.hpp"
#include "SearchEngine.hpp"
#include "DownloadManager.hpp"
#include "RecommendationEngine.hpp"

using namespace std;

class ConnectionManager{
    sqlite3* db;

    //retea
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen;

    //logica
    SearchEngine* searchEngine;
    DownloadManager* downloadManager;
    RecommendationEngine* recommendationEngine; 
    
    //management ul sesiunii
    unordered_map<int, int> socketToUserId;
    public: 
        ConnectionManager(sqlite3* database, SearchEngine* se, DownloadManager* dm, RecommendationEngine* re);
        void run();
};