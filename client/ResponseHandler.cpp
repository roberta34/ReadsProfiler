#include "ResponseHandler.hpp"
#include <iostream> 

using namespace std;

void ResponseHandler::handleResponse(const string &response){
    if(response.empty()) {
        return;
    }

    int pos=response.find(' ');
    string fWord;

    if(pos==string::npos){
        fWord=response;
    }
    else {
        fWord=response.substr(0,pos);
    }

    cout<<"[ResponseHandler] Server response: ";
    if(fWord=="validate"){
        cout<<response<<endl;
        cout<<"[ResponseHandler] Command accepted by server.\n";
    }
    else if (fWord == "error") {
        cout<<response<<endl;
        cout<<"[ResponseHandler] Server reported an error.\n";
    }
    else {
        cout<<response<<endl;
    }
}
