#include "thread_pool.h"

namespace ramen_bot {

ThreadPool::ThreadPool(size_t worker_size)
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

ThreadPool::~ThreadPool() {
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
}