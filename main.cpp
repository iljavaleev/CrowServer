#include "crow.h"
#include <pqxx/pqxx>
#include "backend/Handlers.hpp"
#include "backend/psql.hpp"


int main(){
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")(Help()).methods(crow::HTTPMethod::GET);
    CROW_ROUTE(app, "/contacts")(GET()).methods(crow::HTTPMethod::GET);
    CROW_ROUTE(app, "/contacts/<int>")(DELETE()).methods(crow::HTTPMethod::DELETE);
    CROW_ROUTE(app, "/contacts/<json>")(POST()).methods(crow::HTTPMethod::POST);
    app.port(8000).run();
    return 0;
}