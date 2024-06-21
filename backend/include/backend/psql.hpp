#ifndef psql_h
#define psql_h
#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include <format>
#include <memory>
#include <vector>
#include <future>

class SQL
{
public:
    SQL()
    {
        URI = std::format(
            "dbname={} user={} password={} host={} port={}", 
            std::getenv("POSTGRES_DB") ? std::getenv("POSTGRES_DB") : "postgres", 
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


struct DbObject{
    DbObject(const std::shared_ptr<SQL> db): database{db}{};
    std::shared_ptr<SQL> database;
    virtual ~DbObject() = default;
    virtual DbObject& construct(const pqxx::row&) = 0;

    virtual std::shared_ptr<DbObject> get(int id, const std::string&) = 0;
    virtual std::vector<std::shared_ptr<DbObject>> get_all(const std::string&) = 0;
    
    virtual void create(const std::string&) = 0;
    virtual void create(int id, const std::string&) = 0;
    
    
    virtual void update(const std::string&) = 0;
    virtual void destroy(const std::string&) = 0;
   
};

struct Contact: public DbObject{
    Contact(const std::shared_ptr<SQL> db, const pqxx::row& res): DbObject(db)
    {
        construct(res);
    };
    Contact& construct(const pqxx::row&) override;
    
    std::shared_ptr<DbObject> get(int id, const std::string& qry = "SELECT * FROM contact WHERE id={};") override;
    std::vector<std::shared_ptr<DbObject>> get_all(const std::string& qry = "SELECT * FROM contact;") override;
    
    void update(
        const std::string& query = "UPDATE contact SET id=DEFAULT, name={}, email={}, avatarURL={}, \
        server_user = {} WHERE id = {}"
        ) override;
    void destroy(
        const std::string& query = "DELETE FROM user_lesson WHERE id={}"
        ) override;
    
    void create(
        int idx, 
        const std::string& query = "WITH C as (INSERT INTO contact \
        VALUES (DEFAULT, {}, {}, {}) RETURNING id), \
        SU as (SELECT {} as id) INSERT INTO user_contact (server_user, contact) \
        SELECT  SU.id, T.id FROM C CROSS JOIN SU") override;
    void create(const std::string& query = "INSERT INTO contact VALUES (DEFAULT, {}, {}, {})") override;

    std::vector<std::shared_ptr<DbObject>> get_all_users(
        int idx, 
        const std::string& qry = "WITH uc as (SELECT server_user as id FROM user_contact WHERE contact={}) \
        SELECT server_user.username, server_user.email, server_user.token FROM server_user \
        JOIN uc on uc.id = server_user.id;"
        );

    int id{};
    std::string name{};
    std::string email{};
    std::string avatarURL{};
    int server_user{};
};

struct ServerUser: public DbObject{
    ServerUser(const std::shared_ptr<SQL> db, const pqxx::row& res): DbObject(db)
    {
        construct(res);
    };
    ServerUser& construct(const pqxx::row&) override;
    
    std::shared_ptr<DbObject> get(int id, const std::string& qry = "SELECT * FROM server_user WHERE id={};") override;
    std::vector<std::shared_ptr<DbObject>> get_all(const std::string& qry = "SELECT * FROM server_user;") override;
    
    void update(
        const std::string& query = "UPDATE server_user SET id=DEFAULT, username={}, email={}, token={} WHERE id = {}"
        ) override;
    void destroy(
        const std::string& query = "DELETE FROM user_lesson WHERE id={}"
        ) override;
    
    void create(
        int idx,
        const std::string& query = "WITH SU as (INSERT INTO server_user \
        VALUES (DEFAULT, {}, {}, {}) RETURNING id), \
        C as (SELECT {} as id) \
        INSERT INTO user_contact (server_user, contact) SELECT  C.id, T.id FROM SU CROSS JOIN C") override;
    
    void create(const std::string& query = "INSERT INTO server_user VALUES (DEFAULT, {}, {}, {})") override;

    std::vector<std::shared_ptr<DbObject>> get_all_contacts(
        int idx,
        const std::string& qry = "WITH C as (SELECT contact as id FROM user_contact WHERE server_user={}) \
        SELECT contact.name, contact.email, contact.avatarURL FROM contact \
        JOIN C on C.id = contact.id;"
        );
        
    int id{};
    std::string username{};
    std::string email{};
    std::string token{};
};

#endif 
