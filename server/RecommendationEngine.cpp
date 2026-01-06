#include "RecommendationEngine.hpp"
#include <string>
using namespace std;

RecommendationEngine::RecommendationEngine(sqlite3* database) : db(database) {}

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
     
}

string recommendationsHandler(const string parametres){
    return "There are no recommendations available";
}