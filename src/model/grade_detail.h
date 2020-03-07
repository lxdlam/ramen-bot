#pragma once

#include <fmt/format.h>
#include <fmt/printf.h>

#include "common/common_def.h"

namespace ramen_bot {
struct GradeDetail {
  int gold_perfect = 0;
  int white_perfect = 0;
  int good = 0;
  int bad = 0;
  int miss = 0;

  int total_notes = 0;
  int display_tp = 0;
  double real_tp = 0.;

  static GradeDetail calculate(int tp, int perfect, int good = 0, int bad = 0, int miss = 0) {
    GradeDetail gd;
    int total = gd.total_notes = perfect + good + bad + miss;
    gd.display_tp = tp;
    gd.good = good;
    gd.bad = bad;
    gd.miss = miss;

    int wp = static_cast<int>((1.0 * perfect - 0.0001 * tp * total + 0.3 * good) / 0.3);

    for (int i = std::max(wp - 1, 0); i <= std::min(total, wp + 1); i++) {
      double real_tp = (1.0 * perfect - 1.0 * i + 0.7 * i + 0.3 * good) / total * 100;
      if (static_cast<int>(real_tp * 100) == tp) {
        gd.real_tp = real_tp;
        wp = i;
        break;
      }
    }

    gd.gold_perfect = perfect - wp;
    gd.white_perfect = wp;

    return gd;
  }
};
}  // namespace ramen_bot

namespace fmt {
template <>
struct formatter<ramen_bot::GradeDetail> {
  const static std::string_view EN;
  const static std::string_view CN;

  // 默认 format 成中文
  bool cn = true;

  constexpr auto parse(format_parse_context& ctx) {
    auto it = ctx.begin(), end = ctx.end();
    if (it != end && (*it == 'e' || *it == 'c')) {
      cn = (*it++ == 'c');
    }

    if (it != end && *it != '}') {
      throw format_error("invalid format");
    }

    return it;
  }

  template <typename FormatContext>
  auto format(const ramen_bot::GradeDetail& gd, FormatContext& ctx) {
    return format_to(ctx.out(), cn ? CN : EN, gd.gold_perfect, gd.white_perfect, gd.good, gd.bad, gd.miss,
                     gd.display_tp, gd.real_tp);
  }
};

inline const std::string_view formatter<ramen_bot::GradeDetail>::EN =
    "Result: Gold/Colorful Perfect: {}, While/Black Perfect: {}, Good: {}, Bad: {}, Miss: {}, Display TP: {}, Real TP: "
    "{:.4f}";
inline const std::string_view formatter<ramen_bot::GradeDetail>::CN =
    "结果: 金/彩 Perfect: {}, 白/黑 Perfect: {}, Good: {}, Bad: {}, Miss: {}, 展示 TP: {}, 真实 TP: {:.4f}";
};  // namespace fmt