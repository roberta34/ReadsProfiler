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

    cout<<"[ResponseHandler] Server response: "<<response;

    if(fWord=="validate"){
        cout<<"[ResponseHandler] Command accepted by server.\n";
    }
    else if (fWord == "error")
        cout<<"[ResponseHandler] Server reported an error.\n";
}
