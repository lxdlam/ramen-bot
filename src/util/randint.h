#pragma once

#include <random>

namespace ramen_bot {
class RandomIntGenerator final {
public:
  explicit RandomIntGenerator(int _l, int _r) : engine(rd()), dist(_l, _r) {}

  RandomIntGenerator() = delete;
  RandomIntGenerator(const RandomIntGenerator&) = delete;
  RandomIntGenerator& operator=(const RandomIntGenerator&) = delete;

  int next_int() { return dist(engine); }

  int operator()() { return next_int(); }

private:
  std::random_device rd;
  std::mt19937 engine;
  std::uniform_int_distribution<int> dist;
};
}  // namespace ramen_bot