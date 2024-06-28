#ifndef Queries_hpp
#define Queries_hpp
#include <string>

namespace ContactQueries
{
    inline const std::string get = "SELECT * FROM contact WHERE id={};";
    inline const std::string get_by_token = "SELECT * FROM contact WHERE token={};";
    inline const std::string get_all = "SELECT * FROM contact;";
    inline const std::string create = "WITH C as (INSERT INTO contact \
        VALUES (DEFAULT, {}, {}, {}) RETURNING id), \
        SU as (SELECT {} as id) INSERT INTO user_contact (server_user, contact) \
        SELECT  SU.id, T.id FROM C CROSS JOIN SU;";
    inline const std::string update = "UPDATE contact SET id=DEFAULT, name={}, email={}, avatarURL={}, \
        server_user = {} WHERE id = {}";
    inline const std::string destroy = "DELETE FROM user_lesson WHERE id={}";
    inline const std::string get_all_users = "WITH uc as (SELECT server_user as id FROM user_contact WHERE contact={}) \
        SELECT server_user.username, server_user.email, server_user.token FROM server_user \
        JOIN uc on uc.id = server_user.id;";
};

namespace ServerUserQueries
{
    inline const std::string get = "SELECT * FROM server_user WHERE id={};";
    inline const std::string get_by_token = "SELECT * FROM server_user WHERE token={};";
    inline const std::string get_all = "SELECT * FROM server_user;";
    inline const std::string create_with_contact = "WITH SU as (INSERT INTO server_user \
        VALUES (DEFAULT, {}, {}, {}) RETURNING id), \
        C as (SELECT {} as id) \
        INSERT INTO user_contact (server_user, contact) SELECT  C.id, T.id FROM SU CROSS JOIN C";
    inline const std::string create = "INSERT INTO server_user VALUES (DEFAULT, {}, {}, {})";
    inline const std::string update = "UPDATE server_user SET id=DEFAULT, username={}, email={}, token={} WHERE id = {}";
    inline const std::string destroy = "DELETE FROM user_lesson WHERE id={}";
    inline const std::string get_all_contacts = "WITH C as (SELECT contact as id FROM user_contact WHERE server_user={}) \
        SELECT contact.name, contact.email, contact.avatarURL FROM contact \
        JOIN C on C.id = contact.id;";
};


#endif

