add_library(back
src/Handlers.cpp 
src/psql.cpp
src/Token.cpp
)


target_include_directories(back PUBLIC include) 
