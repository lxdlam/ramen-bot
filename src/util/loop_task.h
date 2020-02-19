#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "common/macro.h"

namespace ramen_bot {
class LoopTask {
public:
  LoopTask() = delete;

  template <typename F, typename Rep, typename Peroid>
  explicit LoopTask(F&&, const std::chrono::duration<Rep, Peroid>&);
  virtual ~LoopTask();

  NO_COPY_MOVE(LoopTask);

private:
  std::thread handle_;
  bool exit_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

template <typename F, typename Rep, typename Peroid>
LoopTask::LoopTask(F&& task, const std::chrono::duration<Rep, Peroid>& duration)
    : exit_(false) {
  handle_ = std::move(std::thread([&]() {
    do {
      task();

      {
        std::unique_lock<std::mutex> lk;
        cv_.wait_for(lk, duration);

        if (exit_) {
          break;
        }
      }
    } while (true);
  }));
}

LoopTask::~LoopTask() {
  {
    std::unique_lock<std::mutex> lk;
    exit_ = true;
  }

  cv_.notify_all();

  if (handle_.joinable()) {
    handle_.join();
  }
}
}  // namespace ramen_bot