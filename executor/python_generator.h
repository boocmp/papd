#pragma once

#include "../project/projects.h"

#include "base_generator.h"

class PythonGenerator : public BaseGenerator {
public:
  explicit PythonGenerator(const Task &task);

  bool Generate() override;

private:
  Task task_;
};
