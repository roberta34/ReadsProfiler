#include "ConnectionManager.hpp"
#include "SearchEngine.hpp"
#include "DownloadManager.hpp"
#include "RecommendationEngine.hpp"
#include "SQLiteStorageLayer.hpp"

int main(){
    SQLiteStorageLayer storage;
    storage.initDatabase();

    SearchEngine searchEngine(storage.getDB());

    DownloadManager downloadManager(storage.getDB());
    
    RecommendationEngine recommendationEngine(storage.getDB());

    ConnectionManager cm(
        &searchEngine,
        &downloadManager,
        &recommendationEngine
    );
    cm.run();
    return 0;
}