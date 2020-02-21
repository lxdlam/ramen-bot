#pragma once

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

    for (int i = std::max(wp - 1, 0); i <= wp + 1; i++) {
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