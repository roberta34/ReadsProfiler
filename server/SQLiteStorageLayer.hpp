#pragma once

#include <sqlite3.h>
#include <string>

using namespace std;

class SQLiteStorageLayer{
    private:
        sqlite3* dataBase;
    public: 
        SQLiteStorageLayer();
        ~SQLiteStorageLayer();
        bool initDatabase();
        bool execute(string &sql);
        sqlite3* getDB();
};