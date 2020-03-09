#include "manager.h"

namespace ramen_bot {
MiddlewareManager::MiddlewareManager() : factory_(MiddlewareFactory::get_instance()) {}

bool MiddlewareManager::configure(const toml::value& val) {
  if (!val.is_table()) {
    return false;
  }

  return true;
}
}  // namespace ramen_bot