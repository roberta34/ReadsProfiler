#include "Protocol.hpp"

string buildResponse(Command type){
    switch(type){
        case Command::SEARCH:
            return "SEARCH Command is valid\n";
        
        case Command::DOWNLOAD:
            return "DOWNLOAD Command is valid\n";
        
        case Command::RECOMMENDATIONS:
            return "RECOMMENDATIONS Command is valid\n";
        
        case Command::LOGIN:
            return "LOGIN Command is valid\n";
        
        case Command::QUIT:
            return "QUIT Command is valid\n";

        default:
            return "Invalid Command\n";
    }
}