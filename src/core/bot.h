#pragma once

#include "common/common_def.h"
#include "common/macro.h"

namespace ramen_bot {
class Bot {
  SINGLETON(Bot);

  bool init();
};
}  // namespace ramen_bot