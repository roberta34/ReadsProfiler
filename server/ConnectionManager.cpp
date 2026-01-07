#include "ConnectionManager.hpp"
#include "RequestParser.hpp"
#include "Protocol.hpp"
#include "SearchEngine.hpp"
#include "DownloadManager.hpp"

#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>
#include <sys/socket.h>

using namespace std;

//obtine id ul unui utilizator dupa username
static int getOrCreateUserId(sqlite3* db, const string& username) {
    {
        const char* sql="SELECT id FROM users WHERE username = ?;";

        sqlite3_stmt* stmt=nullptr;

        if(sqlite3_prepare_v2(db, sql,-1, &stmt, nullptr)!=SQLITE_OK)
            return -1;
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

        int userId=-1;
        if(sqlite3_step(stmt)==SQLITE_ROW) {
            userId=sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        if(userId!=-1) return userId;
    }

    {
        const char* sql="INSERT INTO users(username) VALUES (?);";
        sqlite3_stmt* stmt=nullptr;

        if(sqlite3_prepare_v2(db, sql,-1, &stmt, nullptr)!=SQLITE_OK) 
            return -1;

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

        int rc=sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(rc!=SQLITE_DONE) return -1;
    }

    return (int)sqlite3_last_insert_rowid(db);
}


ConnectionManager::ConnectionManager(sqlite3* database, SearchEngine* se, DownloadManager* dm, RecommendationEngine *re) : db(database), server_fd(0), addrlen(0), searchEngine(se), downloadManager(dm), recommendationEngine(re){
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

static int logSearch(sqlite3* db, int userId, const string& query) {
    const char* sql= "INSERT INTO searches(user_id, query_text) VALUES (?, ?)";

    sqlite3_stmt* stmt=nullptr;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr)!=SQLITE_OK)
        return -1;

    sqlite3_bind_int(stmt,1,userId);
    sqlite3_bind_text(stmt,2,query.c_str(),-1,SQLITE_TRANSIENT);

    int rc=sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(rc!=SQLITE_DONE)
        return -1;

    return(int)sqlite3_last_insert_rowid(db);
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

                    socketToUserId.erase(sd);
                    close(sd);
                    clientSocket[i]=0;
                    
                }
                else{
                    string msg(buffer);

                    cout<<"[Server] Received: "<<msg;

                    Command type=parser.parse(msg);

                    bool loggedIn=socketToUserId.count(sd)>0;

                    if(!loggedIn && type!=Command::LOGIN && type!=Command::QUIT) {
                        string err="error not logged in\n";
                        send(sd, err.c_str(), err.size(),0);
                        continue;
                    }
                    
                    if(type == Command::LOGIN){
                        int pos=msg.find(' ');
                        string username;
                        if(pos!=string::npos) 
                            username=msg.substr(pos+1);
                        while(!username.empty() && (username.back()=='\n' || username.back()=='\r' || username.back()==' ')) 
                            username.pop_back();
                        while(!username.empty() && username.front()==' ')
                            username.erase(username.begin());

                        if (username.empty()) {
                            string errorMsg="error missing username\n";
                            send(sd, errorMsg.c_str(), errorMsg.size(),0);
                            continue;
                        }
                        if (socketToUserId.find(sd)!=socketToUserId.end()) {
                            string errorMsg="error already logged in\n";
                            send(sd, errorMsg.c_str(), errorMsg.size(), 0);
                            continue;
                        }

                        int userId=getOrCreateUserId(db, username);
                        if(userId==-1) {
                            string errorMsg="error login database failure\n";
                            send(sd, errorMsg.c_str(), errorMsg.size(),0);
                            continue;
                        }

                        socketToUserId[sd]=userId;
                        string succesMsg="validate login "+username+" id="+to_string(userId)+"\n";
                        send(sd, succesMsg.c_str(), succesMsg.size(),0);
                        continue;
                    }

                    int userId=socketToUserId[sd];

                    if( type == Command::SEARCH){
                        int pos=msg.find(' ');
                        string filter;
                        if (pos!=string::npos)
                            filter=msg.substr(pos+1);
                        while(!filter.empty() && (filter.back()=='\n' || filter.back()=='\r'))
                            filter.pop_back();

                        int searchId=logSearch(db, userId, filter);

                        if(searchId==-1) {
                            string err="error failed to log search\n";
                            send(sd, err.c_str(), err.size(), 0);
                            continue;
                        }

                        string result=searchEngine->searchHandler(filter);
                        send(sd, result.c_str(), result.size(), 0);
                        continue;
                    }

                    if(type == Command::DOWNLOAD){
                        int pos=msg.find(' ');
                        string id;
                        if(pos!=string::npos)
                            id=msg.substr(pos+1);
                        while(!id.empty() && (id.back()=='\n' || id.back()=='\r' || id.back()==' '))
                            id.pop_back();
                        string result=downloadManager->downloadHandler(userId,id);
                        send(sd, result.c_str(), result.size(),0);
                        continue;
                    }

                    if(type == Command::RECOMMENDATIONS) {
                        string result=recommendationEngine->recommendationsHandler(userId);
                        send(sd, result.c_str(), result.size(), 0);
                        continue;
                    }

                    if(type == Command::QUIT){
                        socketToUserId.erase(sd);
                        close(sd);
                        clientSocket[i]=0;
                        continue;
                    }
                    string err="error invalid command\n";
                    send(sd, err.c_str(), err.size(),0);
                }
            }
        }
    }

}