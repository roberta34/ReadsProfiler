#pragma once 

#include <string>
#include <vector>

using namespace std;

struct Book{
    string title;
    string author;
    string genre;
    int year;
    string ISBN;
    double rating;
    string content;
};

extern vector<Book> bookList;

class SearchEngine{
    public:
        string searchHandler(const string &parametres);
};