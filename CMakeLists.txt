cmake_minimum_required(VERSION 3.20)
project(papd LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(database)
add_subdirectory(executor)
add_subdirectory(project)
add_subdirectory(dflow)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} database executor project dflow)