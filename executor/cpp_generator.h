#pragma once

#include "../project/projects.h"

#include "base_generator.h"

class CppGenerator : public BaseGenerator {
public:
  explicit CppGenerator(const Task &task);

  bool Generate() override;

private:
  void GenerateInput();

  Task task_;
};
