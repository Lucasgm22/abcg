project(cube)
add_executable(${PROJECT_NAME} main.cpp model.cpp window.cpp ground.cpp)
enable_abcg(${PROJECT_NAME})