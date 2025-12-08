#include "ConnectionManager.hpp"
#include "RequestParser.hpp"
#include "Protocol.hpp"

#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>
#include <sys/socket.h>

using namespace std;

ConnectionManager::ConnectionManager(){
    //crearea socketului
    server_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd==0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(8080);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd,3)<0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    cout<<"[ConnectionManager] Server running on port 8080\n";
}

void ConnectionManager::run(){
    fd_set readfds;
    RequestParser parser;
    int clientSocket[30]={0};
    socklen_t addrlen=sizeof(address);
    char buffer[1024];

    while(true){

        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        int max_sd=server_fd;
        for(int i=0;i<30;++i){
            int sd=clientSocket[i];
            if(sd>0){
                FD_SET(sd, &readfds);
                if(sd>max_sd)
                    max_sd=sd;
            }
        }
        int activity=select(max_sd+1, &readfds, NULL,NULL,NULL);
        if(activity<0)
        {
            perror("select error");
            continue;
        }

        if(FD_ISSET(server_fd, &readfds)){
            int newSocket=accept(server_fd, (struct sockaddr*)&address, &addrlen);
            if(newSocket<0){
                perror("accept failed");
                continue;
            }
            cout<<"[Connection Manager] New connection: SD"<<newSocket<<" IP="<<inet_ntoa(address.sin_addr)<<" PORT="<<ntohs(address.sin_port)<<'\n';
            bool added=false;
            for(int &s: clientSocket){
                if(s==0){
                    s=newSocket;
                    added=true;
                    break;
                }
            }
            if(!added){
                cout<<"[ConnectionManager] There are too many clients, rejecting connection";
                close(newSocket);
            }
        }
        for(int i=0;i<30;++i){
            int sd=clientSocket[i];
            if(sd>0 && FD_ISSET(sd, &readfds)){
                memset(buffer, 0, sizeof(buffer));
                int valread=read(sd, buffer, sizeof(buffer)-1);

                if(valread<=0){
                    getpeername(sd, (struct sockaddr*)&address, &addrlen);

                    cout<<"[ConnectionManager] Client disconnected: SD="<<sd<<" IP="<<inet_ntoa(address.sin_addr)<<" PORT="<<ntohs(address.sin_port)<<'\n';

                    close(sd);
                    clientSocket[i]=0;
                }
                else{
                    string msg(buffer);

                    cout<<"[Server] Received: "<<msg;

                    Command type=parser.parse(msg);
                    string response=buildResponse(type);

                    send(sd, response.c_str(), response.size(),0);

                    if(type==Command::QUIT){
                        close(sd);
                        clientSocket[i]=0;
                    }
                }
            }
        }
    }

}