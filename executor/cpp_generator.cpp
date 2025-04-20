#include "cpp_generator.h"

#include <fstream>

#include "../database/database.h"

namespace {

constexpr const char kCMakeTemplate[] = R"cmake(

cmake_minimum_required(VERSION 3.20)
project(${name} LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC "*.h" "*.cpp")
add_executable(${PROJECT_NAME} ${SRC})

)cmake";

class InputGenerator {};

} // namespace

CppGenerator::CppGenerator(const Task &task) : task_(task) {}

bool CppGenerator::Generate() {
  std::string cmake_project = kCMakeTemplate;
  constexpr const char name_placeholder[] = "${name}";
  cmake_project.replace(cmake_project.find(name_placeholder),
                        std::size(name_placeholder) - 1, task_.module.name);
  {
    GenerateInput();
    std::ofstream(task_.module.root / "CMakeLists.txt") << cmake_project;
  }

  {
    std::string generate_project("cmake -B");
    generate_project += task_.module.root.generic_string() + "/build ";
    generate_project += task_.module.root.generic_string();
    if (std::system(generate_project.c_str()) != 0) {
      return false;
    }
  }
  {
    std::string build("cmake --build ");
    build += task_.module.root.generic_string() + "/build ";
    build += "--config=Release ";
    if (!std::system(build.c_str()) != 0) {
      return false;
    }
  }
  return true;
}

void CppGenerator::GenerateInput() {
  std::ofstream input(task_.module.root / "papd_config.h");
  input << "#pragma once" << std::endl << std::endl;
  input << "// GENERATED FILE!" << std::endl << std::endl;

  for (const auto &in : task_.input) {
    for (const auto &value : in.source.GetValues(in.path)) {
      if (value.IsInt()) {
        input << "inline constexpr const int " << value.GetName() << " = "
              << value.GetInt() << ";" << std::endl;
      } else if (value.IsDouble()) {
        input << "inline constexpr const double " << value.GetName() << " = "
              << value.GetDouble() << ";" << std::endl;
      } else if (value.IsString()) {
        input << "inline constexpr const char " << value.GetName() << "[] = \""
              << value.GetString() << "\";" << std::endl;
      }
    }
    input << std::endl;
  }
}
