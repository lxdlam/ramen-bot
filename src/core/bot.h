#pragma once

#include "common/common_def.h"
#include "common/macro.h"
#include "data/resource_context.h"
#include "middleware/manager.h"

namespace ramen_bot {
class Bot {
  SINGLETON(Bot);

public:
  bool init() { return true; };

private:
  ResourceContext& resource_context_;
  MiddlewareManager middleware_manager_;
};
}  // namespace ramen_bot