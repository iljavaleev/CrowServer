#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include "backend/psql.hpp"
#include <format>
#include <exception>
#include <future>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


pqxx::result SQL::insert_into_table(const std::string& query)
{   
    pqxx::result R;
    try
    {
        pqxx::connection c{URI};
        if (c.is_open())
        {
            std::cout << "db is open";
        }
        else
        {
            std::cout << "closed";
        }
        std::cout << std::endl;

        pqxx::work wrk(c);

        R = wrk.exec(query);
        wrk.commit();
        std::cout << "Records created successfully" << std::endl;
        c.close();
        return R;

    }   
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return R;
    }    
}


pqxx::result SQL::select_from_table(const std::string& query)
{   
    pqxx::result R;
    try
    {
        pqxx::connection c{URI};
        if (c.is_open())
        {
            std::cout << "db is open";
        }
        else
        {
            std::cout << "closed";
        }
        std::cout << std::endl;

        pqxx::nontransaction N(c);

        R = N.exec(query);
    
        c.close();
    }   
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return R;      
}


pqxx::result SQL::update(const std::string& query)
{
    pqxx::result R;
    try {
        pqxx::connection C(URI);
        if (C.is_open()) 
        {
        std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } 
        else 
        {
            std::cout << "Can't open database" << std::endl;
        }
        
        pqxx::work W(C);
        R = W.exec(query);
        W.commit();
        C.close();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return R;
}

pqxx::result SQL::destroy(const std::string& query)
{
    pqxx::result R;
    try {
        pqxx::connection C(URI);
        if (C.is_open()) 
        {
        std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } 
        else 
        {
        std::cout << "Can't open database" << std::endl;
        }
        
        pqxx::work W(C);
        
        R = W.exec(query);
        W.commit();
        C.close();
    } 
    catch (const std::exception &e) 
    {
        std::cerr << e.what() << std::endl;
    }
    return R;
}


std::shared_ptr<ServerUser> ServerUser::construct(const pqxx::row& res)
{
    if (res.empty())
        throw std::runtime_error("Empty BD result");
    std::shared_ptr<ServerUser> su{new ServerUser()};
    su->email = res.at(0).as<std::string>();
    su->password = res.at(1).as<std::string>();
    su->salt = res.at(2).as<std::string>();
    return su;
}

std::shared_ptr<Contact> Contact::construct(const pqxx::row& res)
{
    if (res.empty())
        throw std::runtime_error("Empty BD result");
    std::shared_ptr<Contact> c{new Contact};
    c->id = res.at(0).as<int>();
    c->name = res.at(1).as<std::string>();
    c->email = res.at(2).as<std::string>();
    c->avatarURL = res.at(3).as<std::string>();
    return c;
}


std::shared_ptr<UserContact> UserContact::construct(const pqxx::row& res)
{
    if (res.empty())
        throw std::runtime_error("Empty BD result");
    std::shared_ptr<UserContact> uc{new UserContact};
    uc->user_email= res.at(0).as<std::string>();
    uc->contact_id = res.at(1).as<long>();
    return uc;
}


json Contact::to_json()
{
    return json{
        {"id", id},
        {"name", name},
        {"email", email},
        {"avatarURL", avatarURL}
    };
}


json ServerUser::to_json()
{
     return json{
        {"email", email},
        {"password", password},
        {"salt", salt}
    };
}


json UserContact::to_json()
{
     return json{
        {"email", user_email},
        {"contact_id", contact_id}
    };
}

std::string Contact::Get(int idx) 
{ 
    return create_query(ContactQueries::get, idx); 
}
std::string Contact::GetAll() 
{ 
    return ContactQueries::get_all; 
}
std::string Contact::GetAllRelated(int id) 
{ 
    return create_query(ContactQueries::get_all_users, id); 
}
std::string Contact::Destroy(int id) 
{ 
    return create_query(ContactQueries::destroy, id); 
}
std::string Contact::Create(const std::string& user_email) 
{ 
    return create_query(ContactQueries::create, name, email, avatarURL, user_email); 
}
std::string Contact::Update() 
{ 
    return create_query(ContactQueries::update, name, email, avatarURL, id); 
}



std::string ServerUser::Get(const std::string& email) 
{ 
    return create_query(ServerUserQueries::get, email); 
}
std::string ServerUser::GetAll() 
{ 
    return ServerUserQueries::get_all; 
}
std::string ServerUser::GetAllRelated(const std::string& email) 
{ 
    return create_query(ServerUserQueries::get_all_contacts,  email);
}
std::string ServerUser::Destroy(const std::string& email) 
{ 
    return create_query(ServerUserQueries::destroy, email); 
}
std::string ServerUser::Create(int contact_id) 
{ 
    return create_query(ServerUserQueries::create_with_contact, email, password, contact_id); 
}
std::string ServerUser::Create() 
{ 
    return create_query(ServerUserQueries::create, email, password); 
}
std::string ServerUser::Update() 
{ 
    return create_query(ServerUserQueries::update, email, password, email); 
}
