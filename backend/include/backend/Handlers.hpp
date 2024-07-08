#ifndef Handlers_hpp
#define Handlers_hpp
#include <string>
#include "crow.h"
#include "backend/AuthMiddlware.hpp"


struct Help{
    std::string operator()() const;
};

struct Contacts{
    Contacts(crow::App<AuthMW, ContactMW>& _app):app(_app){}
    std::string operator()(const crow::request&) const;
private:
    crow::App<AuthMW, ContactMW>& app;
};

struct DeleteContacts{
    DeleteContacts(crow::App<AuthMW, ContactMW>& _app):app(_app){}
    std::string operator()(const crow::request& req, const int id) const;
private:
    crow::App<AuthMW, ContactMW>& app;
};

struct Register{
    std::string operator()(const crow::request& req) const;
};

struct Token{
    std::string operator()(const crow::request& req) const;
};

struct AddContact{
    AddContact(crow::App<AuthMW, ContactMW>& _app):app(_app){}
    std::string operator()(const crow::request&) const;
private:
    crow::App<AuthMW, ContactMW>& app;
};

#endif