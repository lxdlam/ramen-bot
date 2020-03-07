#pragma once

#include <fmt/core.h>
#include <fmt/format.h>

namespace fmt {
/*
 * std::pair formatter
 * 默认是 first,second
 * 可用 option: {:<first>|:<second>|:<seprator>}，缺失任何一个就使用默认参数，可指定各项参数
 * 如果打印 map 的话可以：;
 * ```c++
 * std::map<int, std::string> m{{1, "hello"}, {2, "world"}};
 * fmt::print("[({})]", fmt::join(m, "),(")); // "[(1,hello),(2,world)]"
 * ```
 */
template <typename T1, typename T2>
struct formatter<std::pair<T1, T2>> {
  enum class Stage {
    kFirstOption,
    kSecondOption,
    kSeparator,
  } stage = Stage::kFirstOption;

  std::string first_option = "", second_option = "", sep = ",";
  std::once_flag first_flag, second_flag, sep_flag;

  constexpr auto parse(format_parse_context &ctx) {
    auto it = ctx.begin(), end = ctx.end();

    auto advance = [&]() {
      if (stage == Stage::kFirstOption) {
        stage = Stage::kSecondOption;
      } else if (stage == Stage::kSecondOption) {
        stage = Stage::kSeparator;
      } else {
        throw format_error("invalid option");
      }
      it++;
    };

    while (it != ctx.end() && *it != '}') {
      if (*it == '|') {
        it++;
        if (*it == ':') {
          advance();
          continue;
        } else {
          it--;
        }
      }
      switch (stage) {
        case Stage::kFirstOption:
          std::call_once(first_flag, [&]() { first_option = ":"; });
          first_option += *it++;
          break;
        case Stage::kSecondOption:
          std::call_once(second_flag, [&]() { second_option = ":"; });
          second_option += *it++;
          break;
        case Stage::kSeparator:
          std::call_once(sep_flag, [&]() { sep = ""; });
          sep += *it++;
          break;
      }
    }

    if (it != end && *it != '}') {
      throw format_error("invalid option");
    }

    return it;
  }

  template <typename FormatContext>
  auto format(const std::pair<T1, T2> &p, FormatContext &ctx) {
    return format_to(ctx.out(), fmt::format("{{{}}}{}{{{}}}", first_option, sep, second_option), p.first, p.second);
  }
};
}  // namespace fmt