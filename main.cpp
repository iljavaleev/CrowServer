#include "crow.h"
#include <pqxx/pqxx>
#include "backend/Handlers.hpp"
#include "backend/psql.hpp"
#include "backend/AuthMiddlware.hpp"


int main(){
    crow::App<AuthMW, ContactMW> app;
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)(Help());
    CROW_ROUTE(app, "/contacts").CROW_MIDDLEWARES(app, ContactMW).methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)(Contacts(app));
    CROW_ROUTE(app, "/contacts/<int>").CROW_MIDDLEWARES(app, ContactMW).methods(crow::HTTPMethod::DELETE)(DeleteContacts(app));
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)(Register());
    CROW_ROUTE(app, "/token/create").methods(crow::HTTPMethod::POST)(Token());
    app.port(5001).run();
    return 0;
}