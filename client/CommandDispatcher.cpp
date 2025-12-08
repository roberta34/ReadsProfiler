#include "CommandDispatcher.hpp"

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

CommandDispatcher::CommandDispatcher(const string &serverip, int serverport): socket_fd(-1), ip(serverip), port(serverport){
    memset(&serverAddr, 0, sizeof(serverAddr));
}

CommandDispatcher::~CommandDispatcher(){
    if(socket_fd!=-1)
        close(socket_fd);
}

bool CommandDispatcher::connectToServer(){
    socket_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd<0){
        perror("socket");
        return false;
    }
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(port);

    if(inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr)<=0){
        perror("inet_pton");
        return false;
    }

    if(connect(socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))<0){
        perror("connect");
        return false;
    }

    cout<<"[CommandDispatcher] Connected to "<<ip<<":"<<port<<"\n";
    return true;
}

bool CommandDispatcher::sendCommand(const string &command){
    if(socket_fd<0){
        return false;
    }

    string info=command;
    if(info.empty() || info.back()!='\n'){
        info.push_back('\n');
    }

    int sent=send(socket_fd, info.c_str(), info.size(),0);
    if(sent<=0){
        perror("send");
        return false;
    }
    return true;
}

int CommandDispatcher::getSocketFd() const{
    return socket_fd;
}