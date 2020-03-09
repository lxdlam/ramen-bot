#include "manager.h"

namespace ramen_bot {
MiddlewareManager::MiddlewareManager() : factory_(MiddlewareFactory::get_instance()) {}

bool MiddlewareManager::init(const toml::value& val) {
  if (!val.is_table()) {
    cq::logging::error("MiddlewareManager", "the configuration is not an table.");
    return false;
  }

  for (const auto& item : toml::find_or<std::vector<std::string>>(val, "enabled", {})) {
    if (!factory_.contains(item)) {
      cq::logging::error("MiddlewareManager", fmt::format("bad config entry, no such middleware. name={}", item));
      return false;
    }
    middleware_list_.emplace_back(item);
  }

  return true;
}

bool MiddlewareManager::run(std::shared_ptr<Event> e) {
  auto instances = make_middlewares();
  if (!instances) {
    return false;
  }

  for (auto&& middleware : *instances) {
    if (e->is_blocked()) {
      return true;
    }

    middleware->process(e);
  }

  return true;
}

std::optional<std::vector<std::unique_ptr<Middleware>>> MiddlewareManager::make_middlewares() {
  std::vector<std::unique_ptr<Middleware>> entities;
  for (const auto& key : middleware_list_) {
    auto mw = factory_.make_entity(key);

    if (mw == nullptr) {
      cq::logging::error("MiddlewareManager", fmt::format("make_middlewares failed, key not found. key={}", key));
      return std::nullopt;
    }

    if (auto itr = configurations_.find(key); itr != configurations_.end()) {
      if (!mw->configure(itr->second)) {
        cq::logging::error("MiddlewareManager",
                           fmt::format("make_middlewares failed, configure middleware failed. key={}", key));
        return std::nullopt;
      }
    }

    entities.emplace_back(std::move(mw));
  }

  return entities;
}
}  // namespace ramen_bot