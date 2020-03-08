#include <cqcppsdk/cqcppsdk.h>

#include "core/bot.h"
#include "model/event.h"

CQ_INIT {
  cq::on_enable([]() {
    if (!ramen_bot::Bot::get_instance().init()) {
      cq::logging::error("初始化", "初始化资源失败！");
      std::exit(1);
    }
  });

  cq::on_private_message([](const cq::PrivateMessageEvent& e) {
    ramen_bot::Event event(e);
    e.block();

    ramen_bot::Bot::get_instance().process(std::move(event));
  });
}