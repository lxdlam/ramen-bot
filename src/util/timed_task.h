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

inline TimedTaskManager::TimedTaskManager(size_t worker_size)
    : shutdown_(false), pool_(worker_size) {
  timer_thread_ = std::move(std::thread([&]() {
    do {
      auto current = std::chrono::system_clock::now();
      auto tt = std::chrono::system_clock::to_time_t(current);
      auto tm = *std::localtime(&tt);
      current_clock_ = tm.tm_hour * 10000 + tm.tm_min * 100 + tm.tm_sec;
      pool_.enqueue([&]() { invoke_tasks(current_clock_); });

      {
        std::unique_lock<std::mutex> lk(exit_mtx_);
        cv_.wait_for(lk, std::chrono::seconds(1));
        if (shutdown_) {
          break;
        }
      }

    } while (true);
  }));
}

inline TimedTaskManager::~TimedTaskManager() {
  {
    std::unique_lock<std::mutex> lk(exit_mtx_);
    shutdown_ = true;
  }

  cv_.notify_all();

  if (timer_thread_.joinable()) {
    timer_thread_.join();
  }
}

inline void TimedTaskManager::register_task(int64_t time,
                                            std::function<void()>&& f) {
  std::unique_lock<std::shared_mutex> lk(task_mtx_);
  auto& vec = task_container_[time];
  vec.emplace_back(std::forward<std::function<void()>>(f));
}

inline void TimedTaskManager::invoke_tasks(int64_t time) {
  std::printf("Current Time: %" PRId64 "\n", time);
}
}  // namespace ramen_bot