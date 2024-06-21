#include "backend/Handlers.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <crow.h>
#include "backend/psql.hpp"
#include <iostream>
#include <memory>

static std::shared_ptr<SQL> db = std::make_shared<SQL>();

std::string HW::operator()() const
{
    std::string q = "select * from accounts limit 10";
    auto r = db->select_from_table(q);
    for (auto i{r.begin()}; i != r.end(); ++i)
    {
        for (auto j{i.begin()}; j!=i.end(); ++j)
        {
            std::cout << *j << " ";
        }
        std::cout << std::endl;
    }
    return q;
} 


std::string WP::operator()(int a, int b) const
{
    return std::to_string(a + b);
} 

std::string JS::operator()() const
{   
    json s;
    s["a"] = "b";
    return s.dump(4);
}