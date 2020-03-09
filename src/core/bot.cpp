#include "bot.h"

#include <fmt/format.h>

#include "model/grade_detail.h"

namespace ramen_bot {
Bot::Bot() : resource_context_(ResourceContext::get_instance()), pool_(100) {}

bool Bot::init() { return true; }

void Bot::process(std::shared_ptr<Event> e) {
#ifdef RELEASE
  pool_.enqueue([this, e = std::move(e)]() { this->process_real(e); });
#else
  process_real(std::move(e));
#endif
}

void Bot::process_real(std::shared_ptr<Event> e) {
  auto re = e->get_event_as<cq::PrivateMessageEvent>();
  cq::send_message(re.target, fmt::format("A message is received. Raw message: {}", re.message));
}
}  // namespace ramen_bot