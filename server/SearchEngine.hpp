#pragma once 

#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

class SearchEngine{
    private:
        sqlite3* db;
    public:
        explicit SearchEngine(sqlite3* database);
        string searchHandler(const string &parameters, vector<int>& resultBookIds);
};