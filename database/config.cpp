#include "config.h"

namespace db {

Value FromJson(const std::string &name, const json::json &v) {
  if (v.is_number_float()) {
    return Value(name, v.get<double>());
  } else if (v.is_number()) {
    return Value(name, v.get<int>());
  } else if (v.is_string()) {
    return Value(name, v.get<std::string>());
  }
  return {};
}

Config::Config(const json::json &cfg) : cfg_(cfg) {}

db::Value Config::GetValue(std::string_view path) const {
  return FromJson(std::string(path), cfg_[path]);
}

std::vector<db::Value> Config::GetValues(std::string_view path) const {
  return {GetValue(path)};
}

} // namespace db
