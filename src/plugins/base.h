#pragma once

#include "common/common_def.h"

namespace ramen_bot {
class TimedPlugin {
}

class ResponsePlugin {
}

using TimedPluginFactory = Factory<std::string, TimedPlugin>;
using ResponsePluginFactory = Factory<std::string, ResponsePlugin>;
}  // namespace ramen_bot