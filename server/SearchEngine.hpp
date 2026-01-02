#pragma once 

#include <string>

using namespace std;

struct Book{
    string title;
    string author;
    string genre;
    int year;
    string ISBN;
    double rating;
};

class SearchEngine{
    public:
        string searchHandler(const string &parametres);
};