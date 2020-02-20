#pragma once

#include "common/common_def.h"

namespace ramen_bot {
class BasePlugin {
public:
  BasePlugin() = delete;
  virtual ~BasePlugin();

  // 用于对于每个对象的启用和禁用函数
  virtual void enable(int64_t) = 0;
  virtual void disable(int64_t) = 0;

  // 用于全局的启用和禁用函数，只允许 superuser 调用
  virtual void enable_all() = 0;
  virtual void disable_all() = 0;

protected:
  std::string name_;
  std::vector<std::string> commands_;
};
}  // namespace ramen_bot