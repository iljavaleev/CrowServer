#ifndef Handlers_hpp
#define Handlers_hpp
#include <string>
#include "crow.h"


struct Help{
    std::string operator()() const;
};

struct GET{
    std::string operator()(const crow::request&) const;
};

struct DELETE{
    std::string operator()(const int id) const;
};

struct POST{
    std::string operator()(const crow::request&) const;
};



#endif