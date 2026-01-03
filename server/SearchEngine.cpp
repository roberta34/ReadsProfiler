#include "SearchEngine.hpp"
#include <string>
#include <vector>
#include <algorithm> 
#include <iomanip>
#include <sstream>

using namespace std;

vector<Book> bookList={
    {"Enigma Otiliei", "George Calinescu", "Roman", 1938, "9789734601234", 4.5, "textul cartii enigma otiliei"},
    {"Morometii", "Marin Preda", "Roman", 1955, "9789734605676", 4.7, "textul cartii morometii"},
    {"Baltagul", "Mihail Sadoveanu", "Roman", 1930, "9789734607892", 4.2, "textul cartii baltagul"}
};

// Helper trim function
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    if (start == string::npos || end == string::npos) return "";
    return s.substr(start, end - start + 1);
}

string SearchEngine::searchHandler(const string &parametres){
    string result = "";
    vector<pair<string, string>> filters;
    size_t start = 0, end = 0;
    while ((end = parametres.find(' ', start)) != string::npos) {
        string token = parametres.substr(start, end - start);
        size_t eq = token.find('=');
        if (eq != string::npos) {
            string key = trim(token.substr(0, eq));
            string val = trim(token.substr(eq + 1));
            filters.push_back({key, val});
        }
        start = end + 1;
    }
    string token = parametres.substr(start);
    size_t eq = token.find('=');
    if (eq != string::npos) {
        string key = trim(token.substr(0, eq));
        string val = trim(token.substr(eq + 1));
        filters.push_back({key, val});
    }
    if (filters.empty()) {
        for(const auto& book : bookList) {
            string yearStr=to_string(book.year);
            ostringstream oss;
            oss << fixed << setprecision(1) << book.rating;
            string ratingStr=oss.str();
            if(book.title.find(parametres)!=string::npos ||
               book.author.find(parametres)!=string::npos ||
               book.genre.find(parametres)!=string::npos ||
               yearStr.find(parametres)!=string::npos ||
               book.ISBN.find(parametres)!=string::npos ||
               ratingStr.find(parametres)!=string::npos ) {
                result += "Title: " + book.title + ", Author: " + book.author + ", Genre: " + book.genre + ", Year: " + to_string(book.year) + ", ISBN: " + book.ISBN + ", Rating: " + ratingStr + "\n";
            }
        }
    } else {
        for(const auto& book : bookList) {
            bool match = true;
            for(const auto& f : filters) {
                ostringstream oss;
                oss << fixed << setprecision(1) << book.rating;
                string ratingStr=oss.str();
                if(f.first == "title" && trim(book.title) != trim(f.second)) match = false;
                else if(f.first == "author" && trim(book.author) != trim(f.second)) match = false;
                else if(f.first == "genre" && trim(book.genre) != trim(f.second)) match = false;
                else if(f.first == "year" && to_string(book.year) != trim(f.second)) match = false;
                else if(f.first == "ISBN" && trim(book.ISBN) != trim(f.second)) match = false;
                else if(f.first == "rating" && ratingStr != trim(f.second)) match = false;
            }
            if(match) {
                ostringstream oss;
                oss << fixed << setprecision(1) << book.rating;
                string ratingStr=oss.str();
                result += "Title: " + book.title + ", Author: " + book.author + ", Genre: " + book.genre + ", Year: " + to_string(book.year) + ", ISBN: " + book.ISBN + ", Rating: " + ratingStr + "\n";
            }
        }
    }
    if(result.empty())
        result="No book found\n";
    return result;
}