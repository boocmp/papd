#pragma once

#include "value.h"

#include <optional>
#include <string_view>
#include <vector>

namespace db {

class BaseDatabase {
public:
  virtual ~BaseDatabase() = default;

  virtual db::Value GetValue(std::string_view path) const = 0;
  virtual std::vector<db::Value> GetValues(std::string_view path) const = 0;
};

BaseDatabase &Constants();

} // namespace db