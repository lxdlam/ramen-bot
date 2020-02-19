#pragma once

#include <toml11/toml.hpp>

#include "common/common_def.h"
#include "common/macro.h"

namespace ramen_bot {
class ResourceContext final {
  SINGLETON(ResourceContext);
};
}  // namespace ramen_bot