#pragma once

#include "../project/projects.h"

#include "base_generator.h"

class FreeFemGenerator : public BaseGenerator {
public:
  explicit FreeFemGenerator(const Task &task);

  bool Generate() override;

private:
  Task task_;
};
