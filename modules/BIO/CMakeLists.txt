

cmake_minimum_required(VERSION 3.20)
project(BIO LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${SRC})

