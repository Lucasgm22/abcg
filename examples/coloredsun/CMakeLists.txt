project(coloredsun)
add_executable(${PROJECT_NAME} main.cpp window.cpp 
                               board.cpp)
enable_abcg(${PROJECT_NAME})