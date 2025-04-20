#include "fem_generator.h"

#include "python_generator.h"

#include <fstream>

#include "../database/database.h"

namespace {
std::string SnakeToCamel(std::string snake_str) {
  bool next_upper = false;
  std::string camel_str;

  for (char c : snake_str) {
    if (c == '_') {
      next_upper = true;
    } else {
      if (next_upper) {
        camel_str.push_back(static_cast<char>(toupper(c)));
        next_upper = false;
      } else {
        camel_str.push_back(c);
      }
    }
  }

  return camel_str;
}
} // namespace

FreeFemGenerator::FreeFemGenerator(const Task &task) : task_(task) {}

bool FreeFemGenerator::Generate() {
  std::ofstream input(task_.module.root / "papd_config.edp");
  input << "// GENERATED FILE!" << std::endl << std::endl;

  for (const auto &in : task_.input) {
    for (const auto &value : in.source.GetValues(in.path)) {
      if (value.IsInt()) {
        input << "real " << SnakeToCamel(value.GetName()) << " = "
              << value.GetInt() << ";" << std::endl;
      } else if (value.IsDouble()) {
        input << "real " << SnakeToCamel(value.GetName()) << " = "
              << value.GetDouble() << ";" << std::endl;
      } else if (value.IsString()) {
        input << "string " << SnakeToCamel(value.GetName()) << " = \""
              << value.GetString() << "\";" << std::endl;
      }
    }
  }
  return true;
}
