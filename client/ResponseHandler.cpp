#include "ResponseHandler.hpp"
#include <iostream> 
#include <fstream>
#include<cstring>

using namespace std;

void ResponseHandler::handleResponse(const string &response) {

    if (response.empty()) {
        return;
    }

    string resp = response;
    resp.erase(0, resp.find_first_not_of("\r\n "));

    if (resp.rfind("DOWNLOAD_CONTENT_BEGIN", 0) == 0) {

        size_t titlePos = resp.find("TITLE:");
        if (titlePos == string::npos) {
            cout << "[Client] Invalid download response (no TITLE)\n";
            return;
        }

        size_t titleEnd = resp.find('\n', titlePos);
        string title = resp.substr(titlePos + 6, titleEnd - (titlePos + 6));

        while (!title.empty() && (title.back() == '\r' || title.back() == ' '))
            title.pop_back();
        while (!title.empty() && title.front() == ' ')
            title.erase(title.begin());

        size_t contentStart = titleEnd + 1;
        size_t contentEnd = resp.find("DOWNLOAD_CONTENT_END");

        if (contentEnd == string::npos) {
            cout << "[Client] Invalid download response (no END)\n";
            return;
        }

        string content = resp.substr(contentStart, contentEnd - contentStart);

        string filename = title + ".txt";

        ofstream out(filename);
        out << content;
        out.close();

        cout << "[Client] Book downloaded locally as " << filename << "\n\n";
        return;
    }

    int pos = resp.find(' ');
    string fWord = (pos == string::npos) ? resp : resp.substr(0, pos);

    cout << "[ResponseHandler] Server response: \n\n";
    if (fWord == "validate") {
        cout << resp << endl;
        cout << "[ResponseHandler] Command accepted by server.\n\n";
    }
    else if (fWord == "error") {
        cout << resp << endl;
        cout << "[ResponseHandler] Server reported an error.\n\n";
    }
    else {
        cout << resp << endl;
    }
}

