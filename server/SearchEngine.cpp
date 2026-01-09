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

SearchEngine::SearchEngine(sqlite3* database): db(database){}

string SearchEngine::searchHandler(const string& parameters, vector<int>&resultBookIds ){
    string sql=
        "SELECT books.id,books.title, authors.name, books.year, books.ISBN, books.rating "
        "FROM books "
        "JOIN authors ON books.author_id=authors.id";
    vector<string> conditions;

    size_t start=0, end;
    while((end=parameters.find(' ', start))!=string::npos) {
        string token=parameters.substr(start, end-start);
        start=end+1;
        int eq=token.find('=');

        if(eq==string::npos) continue; //daca nu exista egal, token ul e ignorat

        string key=trim(token.substr(0,eq));
        string value=trim(token.substr(eq+1));

        if (key=="title")
            conditions.push_back("books.title LIKE '%" + value + "%'");
        else if (key=="author")
            conditions.push_back("authors.name LIKE '%" + value + "%'");
        else if (key=="year")
            conditions.push_back("books.year = " + value);
        else if (key=="ISBN")
            conditions.push_back("books.ISBN = '" + value + "'");
        else if (key=="rating")
            conditions.push_back("books.rating>="+value);
    }
    if (start < parameters.size()) {
        string token = parameters.substr(start);
        int eq = token.find('=');

        if (eq != string::npos) {
            string key = trim(token.substr(0, eq));
            string value = trim(token.substr(eq + 1));

            if (key == "title")
                conditions.push_back("books.title LIKE '%" + value + "%'");
            else if (key == "author")
               conditions.push_back("authors.name LIKE '%" + value + "%'");
            else if (key == "year")
                conditions.push_back("books.year = " + value);
            else if (key == "ISBN")
                conditions.push_back("books.ISBN = '" + value + "'");
            else if (key == "rating")
                conditions.push_back("books.rating >= " + value);
        }
    }


    if (!conditions.empty()) {
        sql+=" WHERE ";
        for(int i=0;i<conditions.size();++i) {
            sql+=conditions[i];
            if(i+1<conditions.size())
                sql+=" AND ";
        }
    }
    sql+=";";
    string result;

    sqlite3_stmt* stmt=nullptr;
    if(sqlite3_prepare_v2(db, sql.c_str(),-1, &stmt, nullptr)!=SQLITE_OK)
        return "SQL prepare error\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int bookId = sqlite3_column_int(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);
        const unsigned char* author = sqlite3_column_text(stmt, 2);
        int year = sqlite3_column_int(stmt, 3);
        const unsigned char* isbn = sqlite3_column_text(stmt, 4);
        double rating = sqlite3_column_double(stmt, 5);

        resultBookIds.push_back(bookId);

        result += "Title: " + string(reinterpret_cast<const char*>(title)) +
                  ", Author: " + string(reinterpret_cast<const char*>(author)) +
                  ", Year: " + to_string(year) +
                  ", ISBN: " + string(reinterpret_cast<const char*>(isbn)) +
                  ", Rating: " + to_string(rating) + "\n";
    }

    sqlite3_finalize(stmt);

    if(result.empty())
        return "No book found\n";
    return result;
}