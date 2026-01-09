#include "SearchEngine.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    if (start == string::npos || end == string::npos) return "";
    return s.substr(start, end - start + 1);
}

SearchEngine::SearchEngine(sqlite3* database) : db(database) {}

string SearchEngine::searchHandler(const string& parameters,vector<int>& resultBookIds)
{
    resultBookIds.clear();
    string params = trim(parameters);
    size_t eq = params.find('=');
    if (eq == string::npos)
        return "Invalid search format. Use key=value\n";

    string key   = params.substr(0, eq);
    string value = params.substr(eq + 1);
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    sqlite3_stmt* stmt=nullptr;
    string sql;
    if (key=="title") {
        sql="SELECT b.id, b.title, a.name, b.year, b.ISBN, b.rating "
            "FROM books b "
            "JOIN authors a ON b.author_id = a.id "
            "WHERE b.title LIKE '%' || ? || '%' COLLATE NOCASE;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    }
    else if (key=="author") {
        sql="SELECT b.id, b.title, a.name, b.year, b.ISBN, b.rating "
            "FROM books b "
            "JOIN authors a ON b.author_id = a.id "
            "WHERE a.name LIKE '%' || ? || '%' COLLATE NOCASE;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    }
    else if (key == "year") {
        int minYear;
        try {
            minYear = stoi(value);
        } catch (...) {
            return "Invalid value for year\n";
        }

        sql =
            "SELECT b.id, b.title, a.name, b.year, b.ISBN, b.rating "
            "FROM books b "
            "JOIN authors a ON b.author_id = a.id "
            "WHERE b.year = ?;";

        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, minYear);
    }

    else if (key=="genre") {
        sql=R"(
            WITH RECURSIVE subgenres(id) AS (
                SELECT id FROM genres WHERE name = ?
                UNION ALL
                SELECT g.id FROM genres g
                JOIN subgenres s ON g.parent_id = s.id
            )
            SELECT DISTINCT b.id, b.title, a.name, b.year, b.ISBN, b.rating
            FROM books b
            JOIN authors a ON b.author_id = a.id
            JOIN book_genres bg ON b.id = bg.book_id
            WHERE bg.genre_id IN (SELECT id FROM subgenres);
        )";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    }
    else if (key=="rating") {
        double minRating;
        try {
            minRating=stod(value);
        } catch(...) {
            return "Invalid value for rating\n";
        }

        sql="SELECT b.id, b.title, a.name, b.year, b.ISBN, b.rating "
            "FROM books b "
            "JOIN authors a ON b.author_id = a.id "
            "WHERE b.rating = ?;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_double(stmt, 1, minRating);
    }
    else if (key=="isbn") {
        sql="SELECT b.id, b.title, a.name, b.year, b.ISBN, b.rating "
            "FROM books b "
            "JOIN authors a ON b.author_id = a.id "
            "WHERE b.ISBN = ?;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    else {
        return "Unknown search criteria\n";
    }

    string result;
    while (sqlite3_step(stmt)==SQLITE_ROW) {
        int id=sqlite3_column_int(stmt,0);
        resultBookIds.push_back(id);

        result+=
            "=============\nTitle: "+string((const char*)sqlite3_column_text(stmt,1))+"\n"+
            "Author: "+string((const char*)sqlite3_column_text(stmt,2))+"\n"+
            "Year: "+to_string(sqlite3_column_int(stmt,3))+"\n"+
            "ISBN: "+string((const char*)sqlite3_column_text(stmt,4))+"\n"+
            "rating: "+to_string(sqlite3_column_double(stmt,5))+"\n=============\n";
    }
    sqlite3_finalize(stmt);
    if (result.empty())
        return "No book found\n";
    else 
        return result;
}