#include "DownloadManager.hpp"

#include <string>
#include <sstream>

DownloadManager::DownloadManager(sqlite3* database) : db(database) {}

string DownloadManager::downloadHandler(int userId,const string &parameters){
       
       string sql=
              "SELECT id, file_path FROM books "
              "WHERE title = ? OR ISBN = ? LIMIT 1;";

       sqlite3_stmt* stmt;
       //statement sql compilat
       if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr)!=SQLITE_OK)
              return "error database prepare failed\n";

       sqlite3_bind_text(stmt, 1, parameters.c_str(), -1, SQLITE_STATIC);
       sqlite3_bind_text(stmt, 2, parameters.c_str(), -1, SQLITE_STATIC);

       int bookId=-1;
       string filePath;

       //step-intreaba baza de date
       //SQLITE_ROW - exista un rand cu date
       if(sqlite3_step(stmt)==SQLITE_ROW) {   
              bookId=sqlite3_column_int(stmt,0); //transforma prima coloana din rand in int
              const unsigned char* text=sqlite3_column_text(stmt,1);
              if(text)
                     filePath=reinterpret_cast<const char*>(text);
       }

       sqlite3_finalize(stmt);

       if(bookId==-1)
              return "error book not found\n";

       string logSQL=
              "INSERT INTO downloads (user_id, book_id) VALUES (?, ?);";

       sqlite3_stmt* logStmt;
       if(sqlite3_prepare_v2(db, logSQL.c_str(), -1, &logStmt, nullptr)==SQLITE_OK) {
              sqlite3_bind_int(logStmt, 1, userId);
              sqlite3_bind_int(logStmt, 2, bookId);
              sqlite3_step(logStmt);
              sqlite3_finalize(logStmt);
       }

       return "DOWNLOAD OK: " + filePath + "\n";
}