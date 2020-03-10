#pragma once

namespace ramen_bot {
template <typename T>
class DoubleBuffer {
public:
  DoubleBuffer();
  virtual ~DoubleBuffer() = default;

  bool init();

  const T& get_container();

protected:
  // How data reloads
  virtual bool load(T& container) = 0;

  int get_idx();

private:
  bool do_load();

private:
  T container_[2];
  std::atomic<int> idx_;

  IntervalLooper looper_;
};

template <typename T>
DoubleBuffer<T>::DoubleBuffer() : idx_(0) {}

template <typename T>
const T& DoubleBuffer<T>::get_container() {
  return container_[get_idx()];
}

template <typename T>
bool DoubleBuffer<T>::init() {
  if (!do_load()) {
    return false;
  }

  looper_.run(std::chrono::seconds(30), [&]() { return do_load(); });

  return true;
}

template <typename T>
bool DoubleBuffer<T>::do_load() {
  int write_pos = get_idx() ^ 1;
  if (!load(write_pos)) {
    return false;
  }
  idx_.store(write_pos, std::memory_order_release);
  return true;
}

template <typename T>
int DoubleBuffer<T>::get_idx() {
  return idx_.load(std::memory_order_relaxed);
}

}  // namespace ramen_bot
