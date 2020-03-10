#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "common/macro.h"

namespace ramen_bot {
class IntervalLooper {
public:
  IntervalLooper();
  virtual ~IntervalLooper();

  template <typename Rep, typename Period, typename F, typename... Args>
  void run(const std::chrono::duration<Rep, Period>&, F&&, Args&&...);

  void stop();

  NO_COPY_MOVE(IntervalLooper);

private:
  std::vector<std::thread> threads_;
  bool exit_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

inline IntervalLooper::IntervalLooper() : exit_(false) {}

template <typename Rep, typename Period, typename F, typename... Args>
void IntervalLooper::run(const std::chrono::duration<Rep, Period>& duration, F&& f, Args&&... args) {
  auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
  threads_.emplace_back([&, task = std::move(task), duration]() {
    do {
      if constexpr (std::is_same_v<typename std::result_of<F(Args...)>::type, bool>) {
        if (!task()) {
          break;
        }
      } else {
        task();
      }

      {
        std::unique_lock<std::mutex> lk(mtx_);
        cv_.wait_for(lk, duration);
        if (exit_) {
          break;
        }
      }
    } while (true);
  });
}

inline void IntervalLooper::stop() {
  {
    std::unique_lock<std::mutex> lk;

    if (exit_) {
      return;
    }

    exit_ = true;
  }

  cv_.notify_all();

  for (auto&& thr : threads_) {
    if (thr.joinable()) {
      thr.join();
    }
  }
}

inline IntervalLooper::~IntervalLooper() { stop(); }
}  // namespace ramen_bot