#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include "backend/psql.hpp"
#include <format>
#include <exception>
#include <future>


std::mutex mtx;

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

std::shared_ptr<ServerUser> ServerUser::construct(const pqxx::row& res)
{
    std::shared_ptr<ServerUser> su{new ServerUser};
    if (res.empty())
        throw std::runtime_error("Empty BD result");
    su->id = res.at(0).as<int>();
    su->username = res.at(1).as<std::string>();
    su->email = res.at(2).as<std::string>();
    su->token = res.at(3).as<std::string>();
    return su;
}

std::shared_ptr<Contact> Contact::construct(const pqxx::row& res)
{
    std::shared_ptr<Contact> c{new Contact};
    if (res.empty())
        throw std::runtime_error("Empty BD result");
    c->id = res.at(0).as<int>();
    c->name = res.at(1).as<std::string>();
    c->email = res.at(2).as<std::string>();
    c->avatarURL = res.at(3).as<std::string>();
    c->server_user = res.at(4).as<int>();
    return c;
}

template class CRUD<Contact>;
template class CRUD<ServerUser>;

template <typename T> CRUD<T>::CRUD(const std::shared_ptr<SQL> db):database{db}{}

template<typename T> std::shared_ptr<T> CRUD<T>::get(int id)
{

    std::string q = T::Get(id);
    std::lock_guard<std::mutex> g(mtx);
    pqxx::result res = database->select_from_table(q);
    
    return T::construct(*res.begin());
}

template<typename T> std::vector<std::shared_ptr<T>> CRUD<T>::get_all()
{
    std::vector<std::shared_ptr<T>> objects;
    std::lock_guard<std::mutex> g(mtx);
    pqxx::result res = database->select_from_table(T::GetAll());
    for (auto i{res.begin()}; i != res.end(); ++i)
    {
        objects.emplace_back(T::construct(*i));
    }
       
    return objects;
}

template<typename T> void CRUD<T>::update(const T& inst)
{   
    std::lock_guard<std::mutex> g(mtx);
    std::string q = inst.Update();
    database->update(q);
}

template<typename T> void CRUD<T>::destroy(int id)
{
    std::lock_guard<std::mutex> g(mtx);
    std::string q = T::Destroy(id);
    database->destroy(q);
}

template<typename T> void CRUD<T>::create(const T& inst)
{
    
    std::lock_guard<std::mutex> g(mtx);
    std::string q = inst.Create();
    database->insert_into_table(q);
}

template<typename T> void CRUD<T>::create(const T& inst, int other_id)
{
    std::string q = inst.Create(other_id);
    std::lock_guard<std::mutex> g{mtx};
    database->insert_into_table(q);
}