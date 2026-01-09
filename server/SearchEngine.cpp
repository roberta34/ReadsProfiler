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

string SearchEngine::searchHandler(const string& parameters,vector<int>& resultBookIds)
{
    string params = trim(parameters);

    if (params.rfind("title=", 0) == 0) {
        string value = trim(params.substr(6));

        string sql =
            "SELECT b.id, b.title, a.name, b.year, b.ISBN, b.rating "
            "FROM books b "
            "JOIN authors a ON b.author_id = a.id "
            "WHERE b.title LIKE '%" + value + "%' COLLATE NOCASE;";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            return "SQL prepare error\n";

        string result;
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

        return result.empty() ? "No book found\n" : result;
    }

    if (params.rfind("author=", 0) == 0) {
        string value = trim(params.substr(7));

        string sql =
            "SELECT b.id, b.title, a.name, b.year, b.ISBN, b.rating "
            "FROM books b "
            "JOIN authors a ON b.author_id = a.id "
            "WHERE a.name LIKE '%" + value + "%' COLLATE NOCASE;";

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            return "SQL prepare error\n";

        string result;
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

        return result.empty() ? "No book found\n" : result;
    }


    if (params.rfind("genre=", 0) == 0) {
        string value = trim(params.substr(6));

        string sql = R"(
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

        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
            return "SQL prepare error\n";

        sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_TRANSIENT);

        string result;
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

        return result.empty() ? "No book found\n" : result;
    }

    string sql =
        "SELECT b.id, b.title, a.name, b.year, b.ISBN, b.rating "
        "FROM books b "
        "JOIN authors a ON b.author_id = a.id;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return "SQL prepare error\n";

    string result;
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

    return result.empty() ? "No book found\n" : result;
}
