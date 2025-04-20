#include "value.h"

namespace db {

Value::Value() = default;
Value::Value(const std::string &name, const std::string &v)
    : name_(name), data_(v) {}
Value::Value(const std::string &name, double v) : name_(name), data_(v) {}
Value::Value(const std::string &name, int v) : name_(name), data_(v) {}

Value::~Value() = default;

bool Value::IsValid() const {
  return !name_.empty() && !std::holds_alternative<std::monostate>(data_);
}

bool Value::IsString() const {
  return std::holds_alternative<std::string>(data_);
}

bool Value::IsDouble() const { return std::holds_alternative<double>(data_); }
bool Value::IsInt() const { return std::holds_alternative<int>(data_); }

bool Value::IsNumber() const { return IsDouble() || IsInt(); }

const std::string &Value::GetName() const { return name_; }

std::string Value::GetString() const {
  if (IsString()) {
    return std::get<std::string>(data_);
  } else if (IsInt()) {
    return std::to_string(GetInt());
  } else if (IsDouble()) {
    return std::to_string(GetDouble());
  }
  static std::string empty;
  return empty;
}

double Value::GetDouble() const {
  if (IsNumber()) {
    return std::get<double>(data_);
  }
  return 0.0;
}

int Value::GetInt() const {
  if (IsNumber()) {
    return std::get<int>(data_);
  }
  return 0;
}

} // namespace db
