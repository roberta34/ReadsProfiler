#pragma once

#include <string>
#include "Protocol.hpp"

using namespace std;

class RequestParser{
    public:
        Command parse(const string &message);
};