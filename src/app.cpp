#include <cqcppsdk/cqcppsdk.h>

CQ_INIT {
  cq::on_private_message([](const cq::PrivateMessageEvent &e) {
    if (e.message == "你好") {
      try {
        cq::send_message(e.target, "你也好～");
      } catch (cq::ApiError &) {
      }
    }
  });
}