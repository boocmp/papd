#pragma once

#include <string>
#include <string_view>
#include <variant>

namespace db {

class Value final {
public:
  Value();
  Value(const std::string &name, const std::string &v);
  Value(const std::string &name, double v);
  Value(const std::string &name, int v);

  ~Value();

  bool IsValid() const;
  bool IsString() const;
  bool IsDouble() const;
  bool IsInt() const;
  bool IsNumber() const;

  const std::string &GetName() const;
  std::string GetString() const;
  double GetDouble() const;
  int GetInt() const;

private:
  std::string name_;
  std::variant<std::monostate, std::string, double, int> data_;
};

} // namespace db
