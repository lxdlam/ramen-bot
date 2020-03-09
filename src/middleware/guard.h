#pragma once

#include "marco.h"

namespace ramen_bot {
DEFINE_MIDDLEWARE_BEGIN(GuardMiddleware, "guard")

bool process(std::shared_ptr<Event>) override final;
bool configure(const toml::value&) override final;

DEFINE_MIDDLEWARE_END(GuardMiddleware, "guard")

inline bool GuardMiddleware::process(std::shared_ptr<Event> e) {
  if (e->is_private_message()) {
    auto& re = e->get_event_as<cq::PrivateMessageEvent>();
    cq::send_message(re.target, fmt::format("enter guard middleware, message id: {}, message: {}", e, re.message));
    return true;
  }
  e->block();
  return true;
}

inline bool GuardMiddleware::configure(const toml::value& cfg) { return true; }
}  // namespace ramen_bot