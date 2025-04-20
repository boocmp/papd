#pragma once

#include "database.h"

#include "../third_party/json/json.h"

namespace db {

class Config : public BaseDatabase {
public:
  explicit Config(const json::json &cfg);

  db::Value GetValue(std::string_view path) const override;
  std::vector<db::Value> GetValues(std::string_view path) const override;

private:
  json::json cfg_;
};

} // namespace db
