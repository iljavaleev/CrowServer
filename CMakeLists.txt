cmake_minimum_required(VERSION 3.5)

project(server)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)  
set(TREADS_PREFER_PTHREAD_FLAG ON)

set(PQXX /usr/local/include/pqxx)
find_library(PQXX_LIB pqxx)


set(JWT_CPP_INCLUDE_DIRS /Users/ilavaleev/vcpkg/packages/jwt-cpp_arm64-osx/include)
set(PICOJSON_INCLUDE_DIRS /Users/ilavaleev/vcpkg/packages/picojson_arm64-osx/include)
find_package(OpenSSL REQUIRED)
find_package(Crow REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

include_directories(/usr/local/include  /opt/homebrew/include/ ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR} ${JWT_CPP_INCLUDE_DIRS} ${PICOJSON_INCLUDE_DIRS})


add_subdirectory(backend)
add_executable(server main.cpp)
target_link_libraries(server back  ${PQXX_LIB} Crow::Crow ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
 