#pragma once

#include "common/common_def.h"
#include "common/type.h"

namespace ramen_bot {
class BasePlugin {
public:
  BasePlugin() = default;
  virtual ~BasePlugin();

  // 用于对于每个对象的启用和禁用函数
  virtual bool enable(int64_t) = 0;
  virtual bool disable(int64_t) = 0;

  // 用于全局的启用和禁用函数，只允许 superuser 调用
  virtual bool enable_all() = 0;
  virtual bool disable_all() = 0;

protected:
  std::string name_;
  std::string help_;
  // 目前暂时不知道是否会出现用户和群 id 重复的情况，暂时合并到一个 list
  std::unordered_set<int64_t> enabled_list_;
  Authority auth;  // 可调用 enable 和 disable 的权限集合
};

class TimedPlugin : public BasePlugin {
public:
  TimedPlugin() = default;
  virtual ~TimedPlugin();

  virtual bool timed_task(int64_t) = 0;
};

class ResponsePlugin : public BasePlugin {
public:
  ResponsePlugin() = default;
  virtual ~ResponsePlugin();

  virtual bool process(int64_t) = 0;

private:
  Authority use_auth;  // 可使用此插件的权限集合
};
}  // namespace ramen_bot