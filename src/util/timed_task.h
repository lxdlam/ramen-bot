#pragma once

#include <cinttypes>
#include <ctime>
#include <shared_mutex>
#include <unordered_map>

#include "common/common_def.h"
#include "common/macro.h"
#include "thread_pool.h"

namespace ramen_bot {
class TimedTaskManager {
public:
  TimedTaskManager() = delete;
  explicit TimedTaskManager(size_t);

  NO_COPY_MOVE(TimedTaskManager);
  virtual ~TimedTaskManager();

  // 时间应该是 HHMMSS 编码
  void register_task(int64_t, std::function<void()>&&);

private:
  void invoke_tasks(int64_t);

private:
  std::thread timer_thread_;
  std::mutex exit_mtx_;
  std::shared_mutex task_mtx_;
  std::condition_variable cv_;
  bool shutdown_;
  int64_t current_clock_;

  ThreadPool pool_;
  std::unordered_map<int64_t, std::vector<std::function<void()>>>
      task_container_;
};
}  // namespace ramen_bot