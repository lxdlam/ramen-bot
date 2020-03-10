#include "guard.h"

namespace ramen_bot {

bool GuardMiddleware::process(std::shared_ptr<Event> e) {
  bool accept = accept_all_events_ || accept_events_.count(fmt::format("{}", e->get_detail_type())) != 0 ||
                accept_events_.count(fmt::format("{}", e->get_source_type())) != 0 ||
                accept_events_.count(fmt::format("{}", e->get_event_type())) != 0;

  if (accept) {
    accept_message(e);
  } else {
    drop_message(e);
  }

  return true;
}

bool GuardMiddleware::configure(const toml::value& cfg) {
  accept_events_.clear();
  for (auto&& item : toml::find_or<std::vector<std::string>>(cfg, "accept_events", {})) {
    if (item == "*") {
      accept_all_events_ = true;
      break;
    }
    accept_events_.emplace(item);
  }

  drop_self_ = toml::find_or<bool>(cfg, "drop_self", true);
  return true;
}

void GuardMiddleware::drop_message(std::shared_ptr<Event> e) {
  fmt::print("message blocked. {:f}\n", e);
  e->block();
}

void GuardMiddleware::accept_message(std::shared_ptr<Event> e) {
  fmt::print("message accepted. {:f}\n", e);
  e->block_raw();
}
}  // namespace ramen_bot