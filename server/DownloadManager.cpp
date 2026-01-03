#include "DownloadManager.hpp"
#include "SearchEngine.hpp"

#include <string>

string downloadHandler(const string &parametres){
       for (const auto& book:bookList){
        if (book.title==parametres || book.ISBN==parametres)
            return book.content;
       }
       return "Book not found";
}