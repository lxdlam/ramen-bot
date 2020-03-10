#pragma once

#include <queue>
#include "common/common_def.h"

namespace ramen_bot {
class Message {
public:
  Message() = default;
  Message(const Message&) = default;
  Message(Message&&) = default;
  Message& operator=(const Message&) = default;
  Message& operator=(Message&&) = default;

  bool parse(const std::string&);

  std::string peek_one();
  std::string cosume_one();

  std::vector<std::string> peek(size_t);
  std::vector<std::string> consume(size_t);

  std::string get_full_raw();
  std::string get_full_cur();

private:
  bool valid_ = false;
  std::string command_;
  std::deque<std::string> payload_;
  std::string raw_;
};
}  // namespace ramen_bot
