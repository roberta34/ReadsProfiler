#pragma once
#include <string>
using namespace std;
enum class Command{
    SEARCH,
    DOWNLOAD, 
    RECOMMENDATIONS,
    LOGIN, 
    QUIT,
    INVALID 
};

string buildResponse(Command type);


