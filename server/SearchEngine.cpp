#include "SearchEngine.hpp"
#include <string>
#include <vector>

using namespace std;

vector<Book> bookList={
    {"Enigma Otiliei", "George Calinescu", "Roman", 1938, "9789734601234", 4.5},
    {"Morometii", "Marin Preda", "Roman", 1955, "9789734605676", 4.7},
    {"Baltagul", "Mihail Sadoveanu", "Roman", 1930, "9789734607892", 4.2}
};

string searchHandler(const string &parametres){
    string result;
    for(const auto& book : bookList) {
        string yearStr=to_string(book.year);
        string ratingStr=to_string(book.rating);
        if(book.title.find(parametres)!=string::npos ||
           book.author.find(parametres)!=string::npos ||
           book.genre.find(parametres)!=string::npos ||
           yearStr.find(parametres)!=string::npos ||
           book.ISBN.find(parametres)!=string::npos ||
           ratingStr.find(parametres)!=string::npos 
    )   {
            result += "Title: " + book.title + ", Author: " + book.author + ", Genre: " + book.genre + ", Year: " + to_string(book.year) + ", ISBN: " + book.ISBN + ", Rating: " + to_string(book.rating) + "\n";
        }
    }
    if(result.empty())
        result="No book found";
    return result;
}