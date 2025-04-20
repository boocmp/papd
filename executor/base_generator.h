#pragma once

#include "../database/value.h"

class BaseGenerator {
public:
  virtual ~BaseGenerator() = default;

  virtual bool Generate() = 0;
};
