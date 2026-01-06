#include "SQLiteStorageLayer.hpp"
#include <iostream>

using namespace std;

SQLiteStorageLayer::SQLiteStorageLayer(): dataBase(nullptr) {
   int rc=sqlite3_open("library.db", &dataBase);
   if(rc!=SQLITE_OK) {
        cerr<<"The database could not be opened\n"<< sqlite3_errmsg(dataBase)<<"\n";
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

bool SQLiteStorageLayer::execute(const std::string &sql) {
    if (dataBase == nullptr) {
        return false;
    }

    char *errorMsg = nullptr;
    int rc = sqlite3_exec(dataBase, sql.c_str(), nullptr, nullptr, &errorMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: ";
        if (errorMsg != nullptr) {
            std::cerr << errorMsg;
            sqlite3_free(errorMsg);
        } else {
            std::cerr << "unknown error";
        }
        std::cerr << '\n';
        return false;
    }

    return true;
}


bool SQLiteStorageLayer::initDatabase() {
    if (!dataBase) 
        return false;

    if(!execute("PRAGMA foreign_keys=ON;"))
        return false;
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

    //==AUTHOR-GENRES==
    const string createAuthorGenres=R"(
        CREATE TABLE IF NOT EXISTS author_genres(
            author_id INTEGER NOT NULL,
            genre_id INTEGER NOT NULL,
            PRIMARY KEY (author_id, genre_id),
            FOREIGN KEY (author_id) REFERENCES authors(id) ON DELETE CASCADE,
            FOREIGN KEY (genre_id) REFERENCES genres(id) ON DELETE CASCADE
        );
    )";

    if(!execute(createAuthorGenres))
        return false;


    //==USERS==
    const string createUsers=R"(
        CREATE TABLE IF NOT EXISTS users(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE
        );
    )";

    if(!execute(createUsers))
        return false;

    //==SEARCHES==
    const string createSearches=R"(
        CREATE TABLE IF NOT EXISTS searches (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            query_text TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        );
    )";

    if(!execute(createSearches))
        return false;

    //==ACCESSED-RESULTS===
    const string createAccessed=R"(
        CREATE TABLE IF NOT EXISTS search_results_accessed (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            search_id INTEGER NOT NULL,
            book_id INTEGER NOT NULL,
            accessed_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (search_id) REFERENCES searches(id) ON DELETE CASCADE,
            FOREIGN KEY (book_id) REFERENCES books(id) ON DELETE CASCADE
        );
    )";

    if(!execute(createAccessed))
        return false;

    //==DOWNLOADS==
    const string createDownloads=R"(
        CREATE TABLE IF NOT EXISTS downloads (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            book_id INTEGER NOT NULL, 
            downloaded_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
            FOREIGN KEY (book_id) REFERENCES books(id) ON DELETE CASCADE
        );
    )";

    if(!execute(createDownloads))
        return false;

    //==RATINGS==
    const string createRatings=R"(
        CREATE TABLE IF NOT EXISTS ratings (
            user_id INTEGER NOT NULL,
            book_id INTEGER NOT NULL,
            rating INTEGER NOT NULL CHECK(rating>=1 AND rating<=5),
            rated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY (user_id, book_id),
            FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE,
            FOREIGN KEY(book_id) REFERENCES books(id) ON DELETE CASCADE
        );
    )";

    if(!execute(createRatings))
        return false;

    //==INDEXES==
    const string createIndexes=R"(
        CREATE INDEX IF NOT EXISTS index_books_title ON books(title); 
        CREATE INDEX IF NOT EXISTS index_books_year ON books(year);
        CREATE INDEX IF NOT EXISTS index_books_ISBN ON books(ISBN);
        CREATE INDEX IF NOT EXISTS index_authors_name ON authors(name);
        CREATE INDEX IF NOT EXISTS index_genres_name ON genres(name);
    )";

    //se creeaza cate un index asupra coloanelor mentionate si astfel cautarea va fi mai rapida: decat sa caute in toata tabela, va cauta doar in coloanele mentionate
    
    if(!execute(createIndexes))
        return false;
    return true;
}   

sqlite3* SQLiteStorageLayer::getDB()const {
    return dataBase;
}