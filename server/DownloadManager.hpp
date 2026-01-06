#pragma once

#include "SearchEngine.hpp"
#include <string>

using namespace std;

class DownloadManager{
    private:
        sqlite3* db;
    public:
        explicit DownloadManager(sqlite3* database); 
        string downloadHandler(int userId,const string &parameters);

};