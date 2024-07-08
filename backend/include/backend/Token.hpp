#ifndef Token_hpp
#define Token_hpp
#include <string>
#include "backend/psql.hpp"

std::string get_token(const std::shared_ptr<ServerUser>& su);
bool verify_token(const std::shared_ptr<ServerUser>& su, const std::string& token);
std::shared_ptr<ServerUser> get_user_if_exists(std::string&& email);

#endif