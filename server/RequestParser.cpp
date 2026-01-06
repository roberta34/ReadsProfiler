    #include "RequestParser.hpp"

    #include <cstring>
    #include <string>
    using namespace std;

    Command RequestParser::parse(const string &message){
        int pos=message.find(' ');
        string cmd=message.substr(0, pos);
        
        if(cmd=="search")
            return Command::SEARCH;

        if(cmd=="download")
            return Command::DOWNLOAD;

        if(cmd=="recommendations")
            return Command::RECOMMENDATIONS;

        if(cmd=="login")
            return Command::LOGIN;

        if(cmd=="quit")
            return Command::QUIT;

        return Command::INVALID;
    }