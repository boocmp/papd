#include "json_database.h"

namespace db {

extern Value FromJson(const std::string &name, const json::json &v);

JsonDatabase::JsonDatabase(const std::filesystem::path &database_file_name)
    : json_file(database_file_name) {}

JsonDatabase::~JsonDatabase() = default;

Value JsonDatabase::GetValue(std::string_view path) const {
  const auto &item = json_file.GetNamed(path);
  return FromJson(item.first, item.second);
}

std::vector<Value> JsonDatabase::GetValues(std::string_view path) const {
  const auto &item = json_file.GetNamed(path);
  if (item.second.is_primitive()) {
    return {FromJson(item.first, item.second)};
  } else if (item.second.is_object()) {
    std::vector<Value> r;
    for (const auto &[name, v] : item.second.items()) {
      auto value = FromJson(name, v);
      if (value.IsValid()) {
        r.push_back(std::move(value));
      }
    }
    return r;
  }
  return {};
}

} // namespace db
