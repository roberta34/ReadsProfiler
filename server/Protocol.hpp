#pragma once
#include <string>
using namespace std;
enum class Command{
    SEARCH,
    DOWNLOAD, 
    RECOMMENDATIONS,
    QUIT,
    INVALID 
};

string buildResponse(Command type);


