#include <cqcppsdk/cqcppsdk.h>

#include "core/bot.h"
#include "model/event.h"

CQ_INIT {
  ramen_bot::Bot::get_instance().init();

  cq::on_private_message([](const cq::PrivateMessageEvent &e) {
    ramen_bot::Event event(e);
    auto raw = event.get_event_as<cq::GroupMessageEvent>();
    try {
      cq::send_message(e.target, e.message);
    } catch (cq::ApiError &) {
    }
  });
}