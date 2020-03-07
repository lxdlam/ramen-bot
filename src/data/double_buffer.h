#pragma once

#include "common/common_def.h"

namespace ramen_bot {
template <typename T>
class DoubleBuffer {
public:
protected:
  // How data reloads
  virtual bool load() = 0;
};

template <typename K, typename V, typename... Args>
class DoubleBufferDict : public virtual DoubleBuffer<std::unordered_map<K, V, Args...>> {};

}  // namespace ramen_bot
