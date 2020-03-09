#include "bot.h"

#include <fmt/format.h>

#include "model/grade_detail.h"

namespace ramen_bot {
Bot::Bot() : resource_context_(ResourceContext::get_instance()), pool_(100) {}

bool Bot::init(const toml::value& config) {
  if (!config.is_table()) {
    cq::logging::error("Bot", "config is not a table");
    return false;
  }

  try {
    if (!middleware_manager_.init(toml::find<toml::value>(config, "middleware"))) {
      cq::logging::error("Bot", "init middleware manager failed.");
      return false;
    }
  } catch (std::out_of_range& e) {
    cq::logging::error("Bot", "middleware section not found in config");
    return false;
  }

  return true;
}

void Bot::process(std::shared_ptr<Event> e) {
#ifdef RELEASE
  pool_.enqueue([this, e = std::move(e)]() { this->process_real(e); });
#else
  process_real(std::move(e));
#endif
}

void Bot::process_real(std::shared_ptr<Event> e) {
  if (!middleware_manager_.run(std::move(e))) {
    cq::logging::error("Bot", fmt::format("run process failed, message id={}", e));
  }
}
}  // namespace ramen_bot