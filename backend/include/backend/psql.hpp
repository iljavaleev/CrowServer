#ifndef psql_h
#define psql_h
#include "Queries.hpp"
#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include <format>
#include <memory>
#include <vector>
#include <future>
#include <boost/uuid/uuid.hpp>           
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


class SQL
{
public:
    SQL()
    {
        URI = std::format(
            "dbname={} user={} password={} host={} port={}", 
            std::getenv("POSTGRES_DB") ? std::getenv("POSTGRES_DB") : "react", 
            std::getenv("POSTGRES_USER") ? std::getenv("POSTGRES_USER") : "postgres", 
            std::getenv("POSTGRES_PASSWORD") ? std::getenv("POSTGRES_PASSWORD") : "postgres",
            std::getenv("POSTGRES_HOST") ? std::getenv("POSTGRES_HOST") : "localhost",
            std::getenv("POSTGRES_PORT") ? std::getenv("POSTGRES_PORT") : "5432");
    }
    std::string URI;
    pqxx::result insert_into_table(const std::string&);
    void update(const std::string&);
    pqxx::result select_from_table(const std::string&);
    void destroy(const std::string&);
};

template<typename T>
class CRUD{
public:
    CRUD(const std::shared_ptr<SQL> db);

    std::shared_ptr<T> get(int id);
    std::vector<std::shared_ptr<T>> get_all();
    
    void create(const T&);
    void update(const T&);
    void create(const T&, int);
    void destroy(int id);
private:
    std::shared_ptr<SQL> database;
};

struct Contact{
    
    int id{};
    std::string name{};
    std::string email{};
    std::string avatarURL{};
    int server_user{};
    
    static std::shared_ptr<Contact> construct(const pqxx::row&);

    static std::string Get(int idx) { return std::vformat(ContactQueries::get, std::make_format_args(idx)); }
    static std::string Get(std::string&& token) { return std::vformat(ContactQueries::get_by_token, std::make_format_args(std::move(token))); }
    static std::string GetAll() { return ContactQueries::get_all; }
    static std::string GetAllUsers(int idx) 
    { 
        return std::vformat(ContactQueries::get_all_users, std::make_format_args(idx)); 
    }
    std::string Create(int user_id) 
    { 
        return std::vformat(ContactQueries::create, std::make_format_args(name, email, avatarURL, user_id)); 
    }
    std::string Update() 
    { 
        return std::vformat(ContactQueries::update, std::make_format_args(name, email, avatarURL, id)); 
    }
    static std::string Destroy(int id) 
    { 
        return std::vformat(ContactQueries::destroy, std::make_format_args(id)); 
    }
};

struct ServerUser{
    
    int id{};
    std::string username{};
    std::string email{};
    std::string get_token()
    {
        if (token.empty())
        {   
            std::stringstream ss;
            std::string  token;
            auto uuid = boost::uuids::random_generator()();
            ss << uuid;
            ss >> token;
        }
        return token;
            
    };

    static std::shared_ptr<ServerUser> construct(const pqxx::row&);
    static std::string Get(int idx) { return std::vformat(ServerUserQueries::get, std::make_format_args(idx)); }
    static std::string Get(std::string&& token) { return std::vformat(ServerUserQueries::get, std::make_format_args(std::move(token))); }
    static std::string GetAll() { return ServerUserQueries::get_all; }
    static std::string GetAllContacts(int idx) 
    { 
        return std::vformat(ServerUserQueries::get_all_contacts, std::make_format_args(idx));
    }
    std::string Create(int contact_id) 
    { 
        return std::vformat(ServerUserQueries::create_with_contact, std::make_format_args(username, email, get_token(), contact_id)); 
    }
    std::string Create() 
    { 
        return std::vformat(ServerUserQueries::create, std::make_format_args(username, email, get_token())); 
    }
    std::string Update() 
    { 
        return std::vformat(ServerUserQueries::update, std::make_format_args(username, email, get_token(), id)); 
    }
    static std::string Destroy(int id) 
    { 
        return std::vformat(ServerUserQueries::destroy, std::make_format_args(id)); 
    }
private:
    std::string token;
};

#endif 
