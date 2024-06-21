#include "crow.h"
#include <pqxx/pqxx>
#include "backend/Handlers.hpp"
#include "backend/psql.hpp"


int main(){
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")(HW());
    CROW_ROUTE(app, "/add/<int>/<int>")(WP());
    CROW_ROUTE(app, "/js")(JS());
    app.port(8000).run();
    return 0;
}