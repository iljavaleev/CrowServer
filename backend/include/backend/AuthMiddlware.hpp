#ifndef AuthMiddlware_hpp
#define AuthMiddlware_hpp
#include "crow.h"
#include "backend/Token.hpp"
#include "jwt-cpp/jwt.h"
#include "jwt-cpp/traits/nlohmann-json/traits.h"

struct AuthMW
{
    struct context
    {};

    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
        auto header = req.headers.find("Authorization"); 
        if ((header == req.headers.end()) && (header->second.empty()))
        {   
            res.code = 403;
            res.end();
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {}
};

struct ContactMW: crow::ILocalMiddleware
{
    struct context
    {
        std::string email{};
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
        auto header = req.headers.find("Authorization");        
        auto token = header->second;
        std::string email;
        try
        {
            auto decoded = jwt::decode(token);
            auto payload = decoded.get_payload_json();
            email = payload.at("email").to_str();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        
        std::shared_ptr<ServerUser> user = get_user_if_exists(std::move(email));
        if (!user || !verify_token(user, token))
        {
            res.code = 403;
            res.end();
        }
        ctx.email = email;
        
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {}
};

#endif