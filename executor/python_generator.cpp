#include "python_generator.h"

#include <fstream>

#include "../database/database.h"

PythonGenerator::PythonGenerator(const Task &task) : task_(task) {}

bool PythonGenerator::Generate() {
  std::ofstream input(task_.module.root / "papd_config.py");
  input << "# GENERATED FILE!" << std::endl << std::endl;

  input << "from collections import namedtuple" << std::endl << std::endl;

  input << "PapdConfigC = namedtuple('Constants', [";

  for (const auto &in : task_.input) {
    for (const auto &value : in.source.GetValues(in.path)) {
      input << "'" << value.GetName() << "'" << ", ";
    }
  }

  input << "])" << std::endl;
  input << "PapdConfig = PapdConfigC(";

  for (const auto &in : task_.input) {
    for (const auto &value : in.source.GetValues(in.path)) {

      if (value.IsInt()) {
        input << value.GetName() << " = " << value.GetInt() << ", ";
      } else if (value.IsDouble()) {
        input << value.GetName() << " = " << value.GetDouble() << ", ";
      } else if (value.IsString()) {
        input << value.GetName() << " = \"" << value.GetString() << "\", ";
      }
    }
  }
  input << ")" << std::endl;
  return true;
}
