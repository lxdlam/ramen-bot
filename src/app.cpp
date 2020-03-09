#include <cqcppsdk/cqcppsdk.h>

#include "common/common_def.h"
#include "core/bot.h"
#include "model/event.h"

using namespace toml::literals::toml_literals;

const toml::value kTestConfig = R"(
[middleware]
enabled = ["guard"]
)"_toml;

CQ_INIT {
  cq::on_enable([]() {
    if (!ramen_bot::Bot::get_instance().init(kTestConfig)) {
      cq::logging::error("on_enable", "global initialize failed");
      std::exit(1);
    }
  });

  cq::on_private_message([](const cq::PrivateMessageEvent& e) {
    e.block();
    auto event = std::make_shared<ramen_bot::Event>(e);

    ramen_bot::Bot::get_instance().process(std::move(event));
  });
}