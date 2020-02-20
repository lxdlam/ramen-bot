#pragma once

#include <cinttypes>
#include <ctime>
#include <list>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>

#include "common/common_def.h"
#include "common/macro.h"
#include "thread_pool.h"

namespace ramen_bot {
struct TaskWrapper {
  int task_id;
  int64_t task_time;
  std::shared_ptr<std::function<void()>> task_ptr;

  TaskWrapper(int _task_id, int64_t _task_time, std::function<void()>&& f)
      : task_id(_task_id), task_time(_task_time) {
    task_ptr = std::make_shared<std::function<void()>>(
        std::forward<std::function<void()>>(f));
  }
};

class TimedTaskManager {
public:
  TimedTaskManager() = delete;
  explicit TimedTaskManager(size_t);

  NO_COPY_MOVE(TimedTaskManager);
  virtual ~TimedTaskManager();

  // 时间应该是 HHMMSS 编码
  int register_task(int64_t, std::function<void()>&&);
  bool remove_task(int);

private:
  void invoke_tasks(int64_t);

private:
  using TaskIter = std::list<TaskWrapper>::iterator;

  struct TaskIterHash {
    size_t operator()(const TaskIter& ti) const {
      return std::hash<int>()(ti->task_id);
    }
  };

  struct TaskIterEqual {
    bool operator()(const TaskIter& lhs, const TaskIter& rhs) const {
      return lhs->task_id == rhs->task_id;
    }
  };

  std::thread timer_thread_;
  std::mutex exit_mtx_;
  std::condition_variable cv_;
  bool shutdown_;
  int64_t current_clock_;
  int task_counter_;

  ThreadPool pool_;
  std::shared_mutex task_mtx_;
  std::list<TaskWrapper> task_container_;
  std::unordered_map<int64_t,
                     std::unordered_set<TaskIter, TaskIterHash, TaskIterEqual>>
      task_timed_map_;
  std::unordered_map<int64_t, TaskIter> task_id_map_;
};

inline TimedTaskManager::TimedTaskManager(size_t worker_size)
    : shutdown_(false), pool_(worker_size), task_counter_(0) {
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

inline int TimedTaskManager::register_task(int64_t time,
                                           std::function<void()>&& f) {
  std::unique_lock<std::shared_mutex> lk(task_mtx_);
  auto task_id = ++task_counter_;
  task_container_.emplace_back(task_id, time,
                               std::forward<std::function<void()>>(f));

  auto itr = task_container_.end();
  itr--;

  task_timed_map_[time].insert(itr);
  task_id_map_.insert({task_id, itr});

  return task_id;
}

inline bool TimedTaskManager::remove_task(int task_id) {
  auto id_itr = task_id_map_.find(task_id);
  if (id_itr == task_id_map_.end()) {
    return false;
  }

  auto task_itr = id_itr->second;
  auto timed_itr = task_timed_map_[task_itr->task_time].find(task_itr);
  if (timed_itr == task_timed_map_[task_itr->task_time].end()) {
    return false;
  }

  task_id_map_.erase(id_itr);
  task_timed_map_[task_itr->task_time].erase(timed_itr);
  task_container_.erase(task_itr);

  return true;
}

inline void TimedTaskManager::invoke_tasks(int64_t time) {
  std::printf("Current Time: %" PRId64 "\n", time);

  auto itr = task_timed_map_.find(time);
  if (itr == task_timed_map_.end()) {
    return;
  }

  for (const auto& task : itr->second) {
    auto task_ptr = task->task_ptr;
    pool_.enqueue([task_ptr = std::move(task_ptr)]() { (*task_ptr)(); });
  }
}
}  // namespace ramen_bot