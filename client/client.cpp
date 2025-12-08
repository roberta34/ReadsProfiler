#include "CommandDispatcher.hpp"
#include "ResponseHandler.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>

using namespace std;

int main() {
    CommandDispatcher dispatcher("127.0.0.1",8080);

    ResponseHandler responsehandler;

    if(!dispatcher.connectToServer())
    {
        cerr<<"Could not connect to server.\n";
        return 1;
    }

    int sock_fd=dispatcher.getSocketFd();
    fd_set fdsread;
    char buffer[1024];
    cout<<"ReadsProfiler client started.\n"<<"Available commands:\n"<<" search <criterii>\n"<<" download <id_carte>\n"<<" recommendations <criterii>\n"<<" quit\n";

    while(true) {
        FD_ZERO(&fdsread);
        FD_SET(STDIN_FILENO, &fdsread);
        FD_SET(sock_fd, &fdsread);

        int max_fd=(STDIN_FILENO>sock_fd) ? STDIN_FILENO:sock_fd;

        int activity=select(max_fd+1, &fdsread, NULL, NULL, NULL);
        if(activity<0) {
            perror("select");
            break;
        }

        if(FD_ISSET(STDIN_FILENO, &fdsread)) {
            memset(buffer, 0, sizeof(buffer));
            int n=read(STDIN_FILENO, buffer, sizeof(buffer)-1);

            if(n<=0){
                cout<<"We closed stdin, exiting\n";
                break;
            }

            string command(buffer);
            if (command=="\n" || command.empty()) {
                cerr<<"Failed to send the command to server.\n";
                break;
            }
            // Trim trailing newline
            if (!command.empty() && command.back() == '\n') {
                command.pop_back();
            }
            // Send command to server
            if (!dispatcher.sendCommand(command)) {
                cerr << "Failed to send the command to server.\n";
                break;
            }
        }

        if (FD_ISSET(sock_fd, &fdsread)) {
            memset(buffer, 0, sizeof(buffer));
            int n=read(sock_fd, buffer, sizeof(buffer)-1);
            if(n<=0) {
                cout<<"Server closed connection.\n";
                break;
            }
            string response(buffer);
            responsehandler.handleResponse(response);
            if(response.rfind("validate quit", 0) == 0) {
                cout << "Quit acknowledged by server, exiting client.\n";
                break;
            }
        }
    }
    return 0;
}