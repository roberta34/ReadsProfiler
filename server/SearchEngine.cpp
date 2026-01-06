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

string SearchEngine::searchHandler(const string& parameters){
    string sql=
        "SELECT books.title, authors.name, books.year, books.ISBN, books.rating"
        "FROM books"
        "JOIN authors ON books.author=authors.id";
    vector<string> conditions;

    int start=0, end;
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
            conditions.push_back("books.author LIKE '%" + value + "%'");
        else if (key=="year")
            conditions.push_back("books.year = " + value);
        else if (key=="ISBN")
            conditions.push_back("books.ISBN = " + value);
        else if (key=="rating")
            conditions.push_back("books.rating>="+value);
    }

    if (!conditions.empty()) {
        sql+=" WHERE ";
        for(int i=0;i<conditions.size();++i) {
            sql+=conditions[i];
            if(i+1<conditions.size())
                sql+="AND";
        }
    }
    sql+=";";
    string result;

    char* errorMessage=nullptr;
    //functie apelata pentru fiecare rand rezultat
    auto callback=[](void* data, int argc, char** argv, char**) -> int {
        string& res=*static_cast<string*>(data);
    }
}