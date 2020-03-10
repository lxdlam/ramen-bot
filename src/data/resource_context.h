#pragma once

#include <toml.hpp>

#include "common/common_def.h"
#include "common/macro.h"
#include "util/timed_task.h"

namespace ramen_bot {
class ResourceContext final {
  SINGLETON(ResourceContext);

public:
  TimedTaskManager timed_task_manager_;
};
}  // namespace ramen_bot