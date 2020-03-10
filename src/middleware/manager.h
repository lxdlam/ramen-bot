#pragma once

#include <fmt/core.h>

#include <toml.hpp>

#include "base.h"
#include "common/common_def.h"
#include "common/macro.h"
#include "guard.h"

namespace ramen_bot {
class MiddlewareManager {
  NO_COPY_MOVE(MiddlewareManager);

public:
  MiddlewareManager();

  bool init(const toml::value&);

  bool run(std::shared_ptr<Event>);

private:
  std::optional<std::vector<std::unique_ptr<Middleware>>> make_middlewares();

private:
  std::vector<std::string> middleware_list_;
  std::unordered_map<std::string, toml::value> configurations_;
  MiddlewareFactory& factory_;
};
}  // namespace ramen_bot