#pragma once

#include "json_internal.h"

#include <filesystem>
#include <fstream>
#include <string_view>

namespace json {

constexpr const char kDelemiter = '.';

using json = nlohmann::json;

class JsonFile {
public:
  explicit JsonFile(const std::filesystem::path &fname)
      : parsed_(json::parse(std::ifstream(fname))) {}

  json Get(std::string_view path) const { return GetNamed(path).second; }

  std::pair<std::string, json> GetNamed(std::string_view path) const {
    json item = parsed_;

    std::string_view::size_type p = 0;
    for (auto i = path.find(kDelemiter); i != std::string_view::npos;
         p = i + 1, i = path.find(kDelemiter, p)) {
      item = item[path.substr(p, i - p)];
    }
    if (path.substr(p) == "*") {
      return {std::string(path), item};
    }
    item = item[path.substr(p)];
    return {std::string(path.substr(p)), item};
  }

private:
  json parsed_;
};

} // namespace json
