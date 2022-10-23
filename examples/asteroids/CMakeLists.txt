project(asteroids)
add_executable(${PROJECT_NAME} main.cpp window.cpp asteroids.cpp bullets.cpp
                               ship.cpp starlayers.cpp)
enable_abcg(${PROJECT_NAME})