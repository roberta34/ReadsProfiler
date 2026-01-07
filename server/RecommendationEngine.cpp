#include "RecommendationEngine.hpp"
#include <string>
#include <sqlite3.h>
using namespace std;


/*recomandarile se vor realiza dupa 3 criterii

    - se recomanda cartile de la autorii cautati de un utilizator

    - se recomanda cartile din acelasi gen cu cartile cautate si descarcate

    - se recomanda cartile cele mai descarcate de la fiecare autor

*/

RecommendationEngine::RecommendationEngine(sqlite3* database) : db(database) {}


//recomandari dupa autorii cautati
void RecommendationEngine::recommendBySearchedAuthors(int userId, string& result, int& count, int limit) {
    string sql = R"(
        SELECT DISTINCT b.id, b.title, b.rating
        FROM books b
        JOIN authors a ON b.author_id=a.id
        WHERE a.id IN (
            SELECT DISTINCT a2.id
            FROM searches s
            JOIN authors a2
                ON s.query_text LIKE '%' || a2.name || '%'
            WHERE s.user_id = ?
        )
        AND b.id NOT IN (
            SELECT book_id FROM downloads WHERE user_id = ?
        )
        ORDER BY b.rating DESC
        LIMIT ?;
    )"; 
     
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr)!=SQLITE_OK)
        return ;

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, userId);
    sqlite3_bind_int(stmt, 3, limit-count);

    while(sqlite3_step(stmt)==SQLITE_ROW && count<limit) {
        const unsigned char* title=sqlite3_column_text(stmt, 1);
        double rating=sqlite3_column_double(stmt, 2);

        result+="[Author-based] ";
        result+=string(reinterpret_cast<const char*>(title));
        result+=" (rating ";
        result+=to_string(rating);
        result+=")\n";

        count++;
    }

    sqlite3_finalize(stmt);
}

//recomandari dupa genurile cautate si descarcate
void RecommendationEngine::recommendByGenres(int userId, string& result, int& count, int limit) {
    string sql=R"(
        SELECT DISTINCT b.id, b.title, b.rating
        FROM books b
        JOIN book_genres bg ON b.id=bg.book_id
        WHERE bg.genre_id IN (
            SELECT bg2.genre_id
            FROM downloads d
            JOIN book_genres bg2 ON d.book_id=bg2.book_id
            WHERE d.user_id=?
        )

        AND b.id NOT IN (
            SELECT book_id FROM downloads WHERE user_id=?
        )
        ORDER BY b.rating DESC
        LIMIT ?;
    )";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr)!=SQLITE_OK) 
        return;
    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, userId);
    sqlite3_bind_int(stmt, 3, limit-count);

    while(sqlite3_step(stmt)==SQLITE_ROW && count<limit) {
        const unsigned char* title=sqlite3_column_text(stmt,1);
        double rating=sqlite3_column_double(stmt,2);

        result+="[Genre-based] ";
        result+=string(reinterpret_cast<const char*>(title));
        result+=" (rating ";
        result+=to_string(rating);
        result+=")\n";

        count++;
    }

    sqlite3_finalize(stmt);
}


//recomandari dupa popularitate

void RecommendationEngine::recommendByPopularity(string& result, int& count, int limit) {
    string sql=R"(
        SELECT b.title, COUNT(d.id) AS cnt
        FROM books b
        LEFT JOIN downloads d ON b.id=d.book_id
        GROUP BY b.id
        ORDER BY cnt DESC
        LIMIT ?
    )";

    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr)!=SQLITE_OK)
        return;
    sqlite3_bind_int(stmt, 1, limit-count);

    while(sqlite3_step(stmt)==SQLITE_ROW && count<limit) {
        const unsigned char* title=sqlite3_column_text(stmt,0);
        int cnt=sqlite3_column_int(stmt,1);

        result+="[Popular] ";
        result+=string(reinterpret_cast<const char*>(title));
        result+=" (downloads ";
        result+=to_string(cnt);
        result+=")\n";

        count++;
    }

    sqlite3_finalize(stmt);
}
string RecommendationEngine::recommendationsHandler(int userId){
    string result;
    int count=0;
    const int LIMIT=10;
    recommendBySearchedAuthors(userId, result, count, LIMIT);

    if(count<LIMIT)
        recommendByGenres(userId, result, count, LIMIT);

    if(count<LIMIT)
        recommendByPopularity(result, count, LIMIT);

    if(result.empty())
        return "No recommendation available\n";

    return result;
}