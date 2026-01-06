#include "SQLiteStorageLayer.hpp"
#include <iostream>

using namespace std;

SQLiteStorageLayer::SQLiteStorageLayer(): database(nullptr) {
   int rc=sqlite3_open("library.db", &dataBase);
   if(rc!=SQLITE_OK) {
        cerr<<"The database could not be opened\n"<< sqlite3_errmsg(database)<<"\n";
        sqlite3_close(dataBase);
        dataBase=nullptr;
   }
   else {
    cout<<"The database opened succesfully.\n";
   }
}

SQLiteStorageLayer::~SQLiteStorageLayer() {
    if(dataBase!=nullptr) {
        sqlite3_close(dataBase);
        dataBase=nullptr;
        cout<<"The connection to the database was closed";
    }
}

bool SQLiteStorageLayer::execute(const string &sql){
    if (dataBase==nullptr) {
        return false;
    }

    char *errorMsg=nullptr;
    int rc=sqlite3_exec(dataBase, sql.c_str(), nullptr, nullptr, &errorMsg);
    //sql.c_str() este SQL-ul ca const char*
    //&errorMsg-aici se pune mesajul de eroare daca apare
    if (rc!=SQLITE_OK) {
        cerr<<"SQLite error: ";
        if (errorMsg!=nullptr)
            cerr<<"unknown error";
        cerr<<'\n';
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool SQLiteStorageLayer::initDatabase() {
    if (!dataBase) 
        return false;

    if(!execute("PRAGMA foreign_keys=ON;"))
        retun false;
    //SQLite are foreign keys dezactivate implicit, astfel le activam

    //==AUTHORS==
    const string createAuthors=R"(
        CREATE TABLE IF NOT EXISTS authors (
            id INTEGER PRIMARY KEY AUTOINCREMENT, 
            name TEXT NOT NULL
        );
    )";
    //AUTOINCREMENT - genereaza automat valori crescatoare pentru id la inserare
    if(!execute(createAuthors))
        return false;


    //==BOOKS==
    const string createBooks=R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author_id INTEGER NOT NULL,
            year INTEGER,
            ISBN TEXT UNIQUE,
            rating REAL DEFAULT 0.0,
            file_path TEXT,
            FOREIGN KEY (author_id) REFERENCES authors(id) ON DELETE CASCADE
        );
    )";
    if(!execute(createBooks))
        return false;

    //Genres with hierarchy
    const string createGenres=R"(
        CREATE TABLE IF NOT EXISTS genres(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            parent_id INTEGER,
            FOREIGN KEY (parent_id) REFERENCES genres(id) ON DELETE SET NULL,
            UNIQUE(name, parent_id)
        );  
    )";

    if(!execute(createGenres))
        return false;

    //==BOOK-GENRES==    
    const string createBookGenres=R"(
        CREATE TABLE IF NOT EXISTS book_genres(
            book_id INTEGER NOT NULL,
            genre_id INTEGER NOT NULL,
            PRIMARY KEY (book_id, genre_id),
            FOREIGN KEY (book_id) REFERENCES books(id) ON DELETE CASCADE,
            FOREIGN KEY (genre_id) REFERENCES genres(id) ON DELETE CASCADE
        );
    )";

    if(!execute(createBookGenres))
        return false;

    const string createAuthorGenres=R"(
        CREATE TABLE IF NOT EXISTS author_genres(
            author_id INTEGER NOT NULL,
            genre_id INTEGER NOT NULL,
            PRIMARY KEY (author_id, genre_id),
            FOREIGN KEY (author_id) REFERENCES authors(id) ON DELETE CASCADE,
            FOREIGN KEY (genre_id) REFERENCES genres(id) ON DELETE CASCADE
        );
    )";
}   

sqlite3* SQLiteStorageLayer::getDB(){
    return dataBase;
}