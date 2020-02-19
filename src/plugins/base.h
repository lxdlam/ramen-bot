#pragma once

#include "common/common_def.h"

namespace ramen_bot {
template <std::string name>
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

private:
  std::string name_ = name;  // 插件名，编译时就必须注明
  std::string command;       // 用于消息分发阶段找到名字的命令
};
}  // namespace ramen_bot