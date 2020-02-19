#include <cqcppsdk/cqcppsdk.h>

CQ_INIT {
  cq::on_private_message([](const cq::PrivateMessageEvent &e) {
    try {
      cq::send_message(e.target, e.message);
    } catch (cq::ApiError &) {
    }
  });
}