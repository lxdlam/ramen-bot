#pragma once

#include <unordered_map>

#include "common/common_def.h"
#include "common/macro.h"
#include "thread_pool.h"

namespace ramen_bot {
class TimedTaskManager {
public:
  using TaskType = std::function<void()>;

  TimedTaskManager() = default;
  NO_COPY_MOVE(TimedTaskManager);
  virtual ~TimedTaskManager();

  // 时间应该是 HHMMSS 编码
  void register_task(int64_t, std::function<void()>);

private:
  void invoke_tasks();

private:
  std::thread timer_thread_;
  std::mutex mtx_;
  std::condition_variable cv_;
  bool shutdown_;
  int64_t current_clock_;

  ThreadPool pool_;
  std::unordered_map<int64_t, std::vector<TaskType>> task_map_;
};
}  // namespace ramen_bot