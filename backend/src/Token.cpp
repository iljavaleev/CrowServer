#include "backend/Token.hpp"
#include "jwt-cpp/jwt.h"
#include "jwt-cpp/traits/nlohmann-json/traits.h"
#include <nlohmann/json.hpp>
#include "backend/psql.hpp"
#include <memory>

using min = std::chrono::minutes;
using sec = std::chrono::seconds;

std::string get_token(const std::shared_ptr<ServerUser>& su)
{
    const auto time = jwt::date::clock::now();
    
    return jwt::create()
    .set_type("JWS")
    .set_issued_at(time)
    .set_not_before(time - sec{15})
	.set_expires_at(time + sec{15} + min{10000})
    .set_issuer("auth0")
    .set_payload_claim("email", jwt::claim(su->email))
    .set_payload_claim("password", jwt::claim(su->password))
    .set_payload_claim("salt", jwt::claim(su->salt))
    .sign(jwt::algorithm::hs256{"secret"});
}


bool verify_token(const std::shared_ptr<ServerUser>& su, const std::string& token)
{
    auto decoded = jwt::decode(token);
    auto verifier = jwt::verify()
    .with_issuer("auth0")
    .with_claim("email", jwt::claim(su->email))
    .with_claim("password", jwt::claim(su->password))
    .with_claim("salt", jwt::claim(su->salt))
    .allow_algorithm(jwt::algorithm::hs256{"secret"});
    
    try
    {
        verifier.verify(decoded);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
    return true;
}


std::shared_ptr<ServerUser> get_user_if_exists(std::string&& email)
{
    std::shared_ptr<ServerUser> res{nullptr};
    try
    {
        res = CRUD<ServerUser>::get(email);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return res;
}