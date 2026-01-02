#include "SearchEngine.hpp"
#include <string>
#include <vector>

using namespace std;

vector<Book> bookList={
    {"Enigma Otiliei", "George Calinescu", "Roman", 1938, "9789734601234", 4.5},
    {"Morometii", "Marin Preda", "Roman", 1955, "9789734605676", 4.7},
    {"Baltagul", "Mihail Sadoveanu", "Roman", 1930, "9789734607892", 4.2}
};

string SearchEngine::searchHandler(const string &parametres){
    string result = "";
    vector<pair<string, string>> filters;
    size_t start = 0, end = 0;
    while ((end = parametres.find(' ', start)) != string::npos) {
        string token = parametres.substr(start, end - start);
        size_t eq = token.find('=');
        if (eq != string::npos) {
            filters.push_back({token.substr(0, eq), token.substr(eq + 1)});
        }
        start = end + 1;
    }
    string token = parametres.substr(start);
    size_t eq = token.find('=');
    if (eq != string::npos) {
        filters.push_back({token.substr(0, eq), token.substr(eq + 1)});
    }
    if (filters.empty()) {
        for(const auto& book : bookList) {
            string yearStr=to_string(book.year);
            string ratingStr=to_string(book.rating);
            if(book.title.find(parametres)!=string::npos ||
               book.author.find(parametres)!=string::npos ||
               book.genre.find(parametres)!=string::npos ||
               yearStr.find(parametres)!=string::npos ||
               book.ISBN.find(parametres)!=string::npos ||
               ratingStr.find(parametres)!=string::npos ) {
                result += "Title: " + book.title + ", Author: " + book.author + ", Genre: " + book.genre + ", Year: " + to_string(book.year) + ", ISBN: " + book.ISBN + ", Rating: " + to_string(book.rating) + "\n";
            }
        }
    } else {
        for(const auto& book : bookList) {
            bool match = true;
            for(const auto& f : filters) {
                if(f.first == "title" && book.title != f.second) match = false;
                else if(f.first == "author" && book.author != f.second) match = false;
                else if(f.first == "genre" && book.genre != f.second) match = false;
                else if(f.first == "year" && to_string(book.year) != f.second) match = false;
                else if(f.first == "ISBN" && book.ISBN != f.second) match = false;
                else if(f.first == "rating" && to_string(book.rating) != f.second) match = false;
            }
            if(match) {
                result += "Title: " + book.title + ", Author: " + book.author + ", Genre: " + book.genre + ", Year: " + to_string(book.year) + ", ISBN: " + book.ISBN + ", Rating: " + to_string(book.rating) + "\n";
            }
        }
    }
    if(result.empty())
        result="No book found\n";
    return result;
}