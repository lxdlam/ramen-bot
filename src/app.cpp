#include <cqcppsdk/cqcppsdk.h>

#include "data/resource_context.h"

CQ_INIT {
  ramen_bot::ResourceContext::get_instance();

  cq::on_private_message([](const cq::PrivateMessageEvent &e) {
    try {
      cq::send_message(e.target, e.message);
    } catch (cq::ApiError &) {
    }
  });
}