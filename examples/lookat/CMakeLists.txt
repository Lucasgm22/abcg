project(lookat)
add_executable(${PROJECT_NAME} camera.cpp ground.cpp main.cpp window.cpp)
enable_abcg(${PROJECT_NAME})