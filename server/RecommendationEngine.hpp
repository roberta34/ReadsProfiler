#pragma once

#include <string>

using namespace std;

class RecommendationEngine{
    private:
        sqlite3* db;

        void recommendBySearchedAuthors (int userId, string& result, int& count, int limit);
        void recommendByGenres(int userId, string& result, int& count, int limit);
        void recommendByPopularity(string& result, int& count, int limit);
    public:
        explicit RecommendationEngine(sqlite3* database);
        string recommendationsHandler(const string &parametres);
};