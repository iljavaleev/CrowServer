#include "backend/Handlers.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <crow.h>
#include "backend/psql.hpp"
#include "backend/AuthMiddlware.hpp"
#include <iostream>
#include <memory>


using json = nlohmann::json;

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


std::string Contacts::operator()(const crow::request& req) const
{   
    const auto& ctx = app.get_context<ContactMW>(req);
    std::string email = ctx.email;
    if (req.method ==  crow::HTTPMethod::GET)
    {
        
        std::vector<std::shared_ptr<Contact>> cp = get_all_related<ServerUser, Contact>(email);
        auto arr = json::array();
        for (std::shared_ptr<Contact> contact: cp)
        {
            arr.emplace_back(contact->to_json());
        }
        return arr.dump();
    }
    json j, resp;
    try
    {
        j = json::parse(req.body);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    if (!j.contains("name") || !j.contains("email") || !j.contains("avatarURL"))
    {   
        resp["code"] = 400; 
        resp["message"] = "wrong payload";
        return  resp.dump();
    }
    Contact contact;
    contact.email = j["email"];
    contact.avatarURL = j["avatarURL"];
    contact.name = j["name"];
    if (CRUD<Contact>::create(contact, email))
    {
        resp = contact.to_json();
        resp.erase("id");
        resp["code"] = 200;
        return resp.dump();
    }    
    resp["code"] = 400;
    return resp.dump();
} 

std::string DeleteContacts::operator()(const crow::request& req, const int id) const
{   
    const auto& ctx = app.get_context<ContactMW>(req);
    std::string email = ctx.email;
    json res;
    try
    {
        CRUD<UserContact>::get(email, id);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        res["code"] = 403;
        res["result"] = "You dont have this contact";
        return res.dump();
    }
    
    std::shared_ptr<UserContact> uc = CRUD<UserContact>::destroy(email, id);
    res = uc->to_json();
    res["code"] = "200";
    return res.dump();
}


std::string Register::operator()(const crow::request& req) const
{   
    json j;
    try
    {
        j = json::parse(req.body);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    json resp;
    if (!j.contains("email") || !j.contains("password"))
    {   
        resp["code"] = 400; 
        resp["message"] = "wrong payload";
        return  resp.dump();
    }
    
    if (CRUD<ServerUser>::get(j["email"]))
    {
        resp["code"] = 400;
        resp["message"] = "you are already have been registered";
        return  resp.dump();
    }

    std::shared_ptr<ServerUser> user;
    try
    {   
        ServerUser su;
        su.email = j["email"]; 
        su.password = j["password"];
        user = CRUD<ServerUser>::create(std::move(su));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    resp = user->to_json();
    resp["code"] = 200;
    resp.erase("salt");

    return resp.dump();
}

std::string Token::operator()(const crow::request& req) const
{   
    
    json j;
    try
    {
        j = json::parse(req.body);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    json resp;
    if (!j.contains("email") || !j.contains("password"))
    {   
        resp["code"] = 400; 
        resp["message"] = "wrong payload";
        return  resp.dump();
    }

    std::shared_ptr<ServerUser> user = CRUD<ServerUser>::get(j["email"]);
     
    if (!CRUD<ServerUser>::get(j["email"]))
    {
        resp["code"] = 400;
        resp["message"] = "You must register first";
        return  resp.dump();
    }
    user = CRUD<ServerUser>::update(*user);
    resp["code"] = 200;
    resp["Token"] = get_token(user);
    
    return resp.dump();
}
