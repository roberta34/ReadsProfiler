#include "DownloadManager.hpp"

#include <string>
#include <fstream>
#include <sstream>

DownloadManager::DownloadManager(sqlite3* database) : db(database) {}

string DownloadManager::downloadHandler(int userId,const string &parameters){
       
       string sql=
              "SELECT id, title, file_path FROM books "
              "WHERE title = ? OR ISBN = ? LIMIT 1;";

       sqlite3_stmt* stmt;
       //statement sql compilat
       if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr)!=SQLITE_OK)
              return "error database prepare failed\n";

       sqlite3_bind_text(stmt, 1, parameters.c_str(), -1, SQLITE_STATIC);
       sqlite3_bind_text(stmt, 2, parameters.c_str(), -1, SQLITE_STATIC);

       int bookId=-1;
       string filePath;
       string title;

       //step-intreaba baza de date
       //SQLITE_ROW - exista un rand cu date
       if(sqlite3_step(stmt)==SQLITE_ROW) {   
              bookId=sqlite3_column_int(stmt,0); 

              const unsigned char* titleText=sqlite3_column_text(stmt,1);
              if(titleText)
                     title=reinterpret_cast<const char*>(titleText);

              const unsigned char* pathText=sqlite3_column_text(stmt,2);
              if(pathText)
                     filePath=reinterpret_cast<const char*>(pathText);
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

       string fullPath="."+filePath;
       ifstream file(fullPath);

       if(!file.is_open())
              return "error file not found on server\n";

       stringstream buffer;
       buffer<<file.rdbuf();
       file.close();

       return "DOWNLOAD_CONTENT_BEGIN\nTITLE:"+title+"\n"+buffer.str()+"\nDOWNLOAD_CONTENT_END\n";
}