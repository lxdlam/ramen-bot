#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

#include "common/common_def.h"
#include "common/macro.h"

namespace ramen_bot {
class ThreadPool {
public:
  ThreadPool() = delete;
  explicit ThreadPool(size_t worker_size);
  virtual ~ThreadPool();

  NO_COPY_MOVE(ThreadPool);

  template <typename F, typename... Args>
  auto enqueue(F&& f, Args&&... args)
      -> std::optional<std::future<typename std::result_of<F(Args...)>::type>>;

private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;

  size_t worker_size_;
  std::mutex mtx_;
  std::condition_variable cv_;
  bool shutdown_;
};

template <typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::optional<std::future<typename std::result_of<F(Args...)>::type>> {
  using R = typename std::result_of<F(Args...)>::type;

  auto pt = std::make_shared<std::packaged_task<R()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<R> ret = pt->get_future();
  {
    std::unique_lock<std::mutex> lk(mtx_);

    if (shutdown_) {
      return std::nullopt;
    }

    tasks_.emplace([pt = std::move(pt)]() { (*pt)(); });
  }

  cv_.notify_one();

  return ret;
}

inline ThreadPool::ThreadPool(size_t worker_size)
    : worker_size_(worker_size), shutdown_(false) {
  for (size_t i = 0; i < worker_size; i++) {
    workers_.emplace_back(std::thread([&]() {
      do {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lk(mtx_);
          cv_.wait(lk, [&]() { return shutdown_ || !tasks_.empty(); });

          if (shutdown_) {
            return;
          }

          task = std::move(tasks_.front());
          tasks_.pop();
        }

        task();
      } while (true);
    }));
  }
}

inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lk(mtx_);
    shutdown_ = true;
  }

  cv_.notify_all();

  for (auto&& worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}
}  // namespace ramen_bot