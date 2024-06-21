#ifndef Handlers_hpp
#define Handlers_hpp
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct HW{
    std::string operator()() const;
};

struct WP{
    std::string operator()(int a, int b) const;
};

struct JS{
    std::string operator()() const;
};



#endif