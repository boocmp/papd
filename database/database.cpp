#include "database.h"

#include "json_database.h"

namespace db {

BaseDatabase &Constants() {
  static JsonDatabase instance("data/constants.json");
  return instance;
}

} // namespace db
