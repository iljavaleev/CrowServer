#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include "backend/psql.hpp"
#include <format>
#include <exception>
#include <future>




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

void SQL::update(const std::string& query)
{
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
        W.exec(query);
        W.commit();
        C.close();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void SQL::destroy(const std::string& query)
{

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
        
        W.exec(query);
        W.commit();
        C.close();
    } 
    catch (const std::exception &e) 
    {
        std::cerr << e.what() << std::endl;
    }
}

ServerUser& ServerUser::construct(const pqxx::row& res)
{
    if (res.empty())
        throw std::runtime_error("Empty BD result");
    id = res.at(0).as<int>();
    username = res.at(1).as<std::string>();
    email = res.at(2).as<std::string>();
    token = res.at(3).as<std::string>();
    return *this;
}

Contact& Contact::construct(const pqxx::row& res)
{
    if (res.empty())
        throw std::runtime_error("Empty BD result");
    id = res.at(0).as<int>();
    name = res.at(1).as<std::string>();
    email = res.at(2).as<std::string>();
    avatarURL = res.at(3).as<std::string>();
    server_user = res.at(4).as<int>();
    return *this;
}

std::shared_ptr<DbObject> Contact::get(int id, const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(id));
    pqxx::result res = this->database->select_from_table(q);
    return std::make_shared<Contact>(construct(*res.begin()));
}

std::shared_ptr<DbObject> ServerUser::get(int id, const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(id));
    pqxx::result res = this->database->select_from_table(q);
    return std::make_shared<ServerUser>(construct(*res.begin()));
}

std::vector<std::shared_ptr<DbObject>> ServerUser::get_all(const std::string& query)
{
    std::vector<std::shared_ptr<DbObject>> objects;
    pqxx::result res = this->database->select_from_table(query);
    for (auto i{res.begin()}; i != res.end(); ++i)
        objects.emplace_back(std::make_shared<ServerUser>(construct(*i)));
    return objects;
}

std::vector<std::shared_ptr<DbObject>> Contact::get_all(const std::string& query)
{
    std::vector<std::shared_ptr<DbObject>> objects;
    pqxx::result res = this->database->select_from_table(query);
    for (auto i{res.begin()}; i != res.end(); ++i)
        objects.emplace_back(std::make_shared<Contact>(construct(*i)));
    return objects;
}

void Contact::update(const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(name, email, avatarURL, id));
    this->database->update(q);
}

void ServerUser::update(const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(username, email, token, id));
    this->database->update(q);
}

void Contact::destroy(const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(id));
    this->database->destroy(q);
}

void ServerUser::destroy(const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(id));
    this->database->destroy(q);
}

void ServerUser::create(const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(username, email, token));
    this->database->insert_into_table(q);
}

void ServerUser::create(int idx, const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(username, email, token, idx));
    this->database->insert_into_table(q);
}

void Contact::create(int idx, const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(name, email, avatarURL, idx));
    this->database->insert_into_table(q);
}

void Contact::create(const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(name, email, avatarURL));
    this->database->insert_into_table(q);
}

std::vector<std::shared_ptr<DbObject>> Contact::get_all_users(int idx, const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(idx));
    std::vector<std::shared_ptr<DbObject>> objects;
    pqxx::result res = this->database->select_from_table(query);
    for (auto i{res.begin()}; i != res.end(); ++i)
        objects.emplace_back(std::make_shared<ServerUser>(construct(*i)));
    return objects;
}

std::vector<std::shared_ptr<DbObject>> ServerUser::get_all_contacts(int idx, const std::string& query)
{
    std::string q = std::vformat(query, std::make_format_args(idx));
    std::vector<std::shared_ptr<DbObject>> objects;
    pqxx::result res = this->database->select_from_table(query);
    for (auto i{res.begin()}; i != res.end(); ++i)
        objects.emplace_back(std::make_shared<Contact>(construct(*i)));
    return objects;
}