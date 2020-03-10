#pragma once

#include <condition_variable>

#include "common/common_def.h"

namespace ramen_bot {
template <typename K, typename V, typename... Args>
class DoubleBufferDict : public virtual DoubleBuffer<std::unordered_map<K, V, Args...>> {};

}  // namespace ramen_bot
