#include "backend/Handlers.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <crow.h>
#include "backend/psql.hpp"
#include <iostream>
#include <memory>


static std::shared_ptr<SQL> db = std::make_shared<SQL>();

std::string Help::operator()() const
{
    const std::string q = "<pre><h3>Welcome to the Address Book API!</h3>\n\r "
    "Use an Authorization header to work with your own data:\n\r "
    "fetch(url, { headers: { 'Authorization': 'whatever-you-want' }})\n\r "
    "The following endpoints are available:\n\r "
    "GET /contacts\n\r "
    "DELETE /contacts/:id\n\r "
    "POST /contacts { name, email, avatarURL } </pre>";
    return q;
} 


std::string GET::operator()(const crow::request& req, const crow::response& res) const
{
    std::string token = req.headers.get("Authorization");
    if(token.empty())
        res.

    return std::to_string(a + b);
} 

std::string DELETE::operator()(const int id) const
{   
    json s;
    s["a"] = "b";
    return s.dump(4);
}

std::string POST::operator()(const crow::request&) const
{   
    json s;
    s["a"] = "b";
    return s.dump(4);
}