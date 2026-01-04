#include "SQLiteStorageLayer.hpp"
#include <iostream>

using namespace std;

SQLiteStorageLayer::SQLiteStorageLayer(){
   int rc=sql3_open("library.db", &dataBase);
   if(rc) {
        cerr<<"The database could not be opened\n";
        dataBase=nullptr;
   }
   else {
    cout<<"The database opened";
   }
}

SQLiteStorageLayer::~SQLiteStorageLayer() {
    if(dataBase) {
        sqlite3_close(dataBase);
        cout<<"The connection to the database was closed";
    }
}

bool SQLiteStorageLayer::initDatabase() {
    if (!dataBase) 
        return false;
    string sql=R"(
        CREATE TABLE IF NOT EXISTS authors (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL
        );
    )";
}

sqlite3* SQLiteStorageLayer::getDB(){
    return dataBase;
}