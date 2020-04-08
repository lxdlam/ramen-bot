#pragma once

#include "common/common_def.h"
#include "model/event.h"
#include "util/factory.h"

namespace ramen_bot {
class Middleware {
public:
  virtual bool process(std::shared_ptr<Event>) = 0;
  virtual bool configure(const toml::value&) = 0;
  virtual ~Middleware() = default;
};

using MiddlewareFactory = Factory<std::string, Middleware>;
}  // namespace ramen_bot