#pragma once

#include <cstring>
#include <netinet/in.h>
#include <string>

using namespace std;

class CommandDispatcher{
        int socket_fd;
        string ip;
        int port;
        struct sockaddr_in serverAddr;
    public:
        CommandDispatcher(const string &serverip, int serverport);
        ~CommandDispatcher();

        bool connectToServer();
        bool sendCommand(const string &);
        int getSocketFd() const;
};