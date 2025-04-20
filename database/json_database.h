#include "database.h"

#include "../third_party/json/json.h"

#include <filesystem>
#include <string>

namespace db {

class JsonDatabase : public BaseDatabase {
public:
  explicit JsonDatabase(const std::filesystem::path &database_file_name);
  ~JsonDatabase() override;

  Value GetValue(std::string_view path) const override;
  std::vector<Value> GetValues(std::string_view path) const override;

private:
  json::JsonFile json_file;
};

} // namespace db
