#ifndef Queries_hpp
#define Queries_hpp
#include <string>

namespace ContactQueries
{
    inline const std::string get = "SELECT * FROM contact WHERE id={};";
    inline const std::string get_all = "SELECT * FROM contact;";
    inline const std::string create = "WITH C as (INSERT INTO contact \
    VALUES (DEFAULT, '{}', '{}', '{}') RETURNING id), \
    SU as (SELECT '{}' as email) INSERT INTO user_contact \
    (user_email, contact) SELECT  SU.email, C.id FROM C CROSS JOIN SU RETURNING contact;";
    inline const std::string update = "UPDATE contact SET id=DEFAULT, name={}, email={}, avatarURL={}, \
        server_user = {} WHERE id = {} RETURNING *";
    inline const std::string destroy = "DELETE FROM user_lesson WHERE id={} RETURNING *";
    inline const std::string get_all_users = "WITH uc as (SELECT user_email as email FROM user_contact WHERE contact={}) \
        SELECT server_user.email, server_user.passw, server_user.salt FROM server_user \
        JOIN uc on uc.email = server_user.email;";
};

namespace ServerUserQueries
{
    inline const std::string get = "SELECT * FROM server_user WHERE email='{}';";
    inline const std::string get_all = "SELECT * FROM server_user;";
    inline const std::string create_with_contact = "WITH SU as (INSERT INTO server_user \
        VALUES ('{}', '{}', gen_random_uuid()) RETURNING user_email), \
        C as (SELECT {} as id) INSERT INTO user_contact (user_email, contact) \
        SELECT SU.email, C.id FROM SU CROSS JOIN C RETURNING user_email;";
    inline const std::string create = "INSERT INTO server_user VALUES ('{}', '{}', gen_random_uuid()) RETURNING *;";
    inline const std::string update = "UPDATE server_user SET email='{}', passw='{}', salt=gen_random_uuid() WHERE email = '{}' RETURNING *;";
    inline const std::string destroy = "DELETE FROM user_lesson WHERE id={} RETURNING *;";
    inline const std::string get_all_contacts = "WITH C as (SELECT contact as id FROM user_contact WHERE user_email='{}') \
        SELECT contact.id, contact.name, contact.email, contact.avatarURL FROM contact \
        JOIN C on C.id = contact.id;";
};

namespace UserContactQueries
{
    inline const std::string get = "SELECT * FROM user_contact WHERE user_email='{}' AND contact={};";
    inline const std::string destroy = "DELETE FROM user_contact WHERE user_email='{}' AND contact={} RETURNING *;";
};

#endif

