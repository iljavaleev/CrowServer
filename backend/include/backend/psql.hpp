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
#include <mutex>
#include <nlohmann/json.hpp>


using json = nlohmann::json;


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
    pqxx::result update(const std::string&);
    pqxx::result select_from_table(const std::string&);
    pqxx::result destroy(const std::string&);
};

template<typename T>
class CRUD{
    template<typename FT, typename RT, typename PK> friend std::vector<std::shared_ptr<RT>> get_all_related(PK pk);
public:
    CRUD() = delete;
    using Tptr = std::shared_ptr<T>;
    template<typename PK> static Tptr get(PK pk)
    {
        std::string q = T::Get(std::move(pk));
        pqxx::result res = database->select_from_table(q);
        return T::construct(*res.begin());
    }
    
    template<typename PK1, typename PK2> static Tptr get(PK1 pk1, PK2 pk2 = PK1())
    {
        std::string q = T::Get(std::move(pk1), std::move(pk2));
        pqxx::result res = database->select_from_table(q);
        return T::construct(*res.begin());
    }
    
    static std::vector<Tptr> get_all()
    {
        std::vector<Tptr> objects;
        pqxx::result res = database->select_from_table(T::GetAll());
        for (auto i{res.begin()}; i != res.end(); ++i)
        {
            objects.emplace_back(T::construct(*i));
        }
        
        return objects;
    }
    
    static Tptr create(T&& inst)
    {
        std::string q = inst.Create();
        pqxx::result res = database->insert_into_table(q);
        return T::construct(*res.begin());
    }
    
    static Tptr update(T& inst)
    {
        std::string q = inst.Update();
        pqxx::result res = database->update(q);
        return T::construct(*res.begin());
    }
    
    template<typename PK> static bool create(T& inst, PK other_pk)
    {  
        std::string q = inst.Create(other_pk);
        pqxx::result res = database->insert_into_table(q);
        if (!res.empty())
            return true;
        return false;
    } 
    
    static Tptr destroy(int id)
    {
        std::string q = T::Destroy(id);
        pqxx::result res = database->destroy(q);
        return T::construct(*res.begin());
    }

    template <typename PK1, typename PK2> static Tptr destroy(const PK1& pk1, const PK2& pk2=PK1())
    {
        std::string q = T::Destroy(pk1, pk2);
        pqxx::result res = database->destroy(q);
        return T::construct(*res.begin());
    }

private:
    const static std::unique_ptr<SQL> database;
};


template<typename T> const std::unique_ptr<SQL> CRUD<T>::database = std::make_unique<SQL>();

template<typename FT, typename RT, typename PK> static std::vector<std::shared_ptr<RT>> get_all_related(PK pk)
{
    std::vector<std::shared_ptr<RT>> objects;
    
    pqxx::result res = CRUD<FT>::database->select_from_table(FT::GetAllRelated(pk));
    for (auto i{res.begin()}; i != res.end(); ++i)
    {
        objects.emplace_back(RT::construct(*i));
    }
    return objects;
}

template<typename ... Args> std::string create_query(const std::string& bluep, Args&&... args)
{
    return std::vformat(bluep, std::make_format_args(args...)); 
}  


struct Contact{
    int id{};
    std::string name{};
    std::string email{};
    std::string avatarURL{};
    static std::shared_ptr<Contact> construct(const pqxx::row&);
    
    static std::string Get(int); 
    static std::string GetAll();
    static std::string GetAllRelated(int);
    static std::string Destroy(int); 
    std::string Create(const std::string& pk); 
    std::string Update(); 
    json to_json();
};

struct ServerUser{
    
    std::string email{};
    std::string password{};
    std::string salt{};

    static std::shared_ptr<ServerUser> construct(const pqxx::row&);

    static std::string Get(const std::string&); 
    static std::string GetAll();
    static std::string GetAllRelated(const std::string&);
    static std::string Destroy(const std::string&); 
    std::string Create(int contact_id); 
    std::string Create(); 
    std::string Update(); 
    json to_json();
};

struct UserContact{
    std::string user_email{};
    long contact_id{};

    static std::shared_ptr<UserContact> construct(const pqxx::row&);
    
    static std::string Get(const std::string& user_email, const int& contact_id) 
    { 
        return create_query(UserContactQueries::get, user_email, contact_id); 
    }
    
    static std::string Destroy(const std::string& user_email,const int& contact_id) 
    { 
        return create_query(UserContactQueries::destroy, user_email, contact_id); 
    }
    json to_json();
};

#endif 
