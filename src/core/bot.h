#pragma once

#include "common/common_def.h"
#include "common/macro.h"
#include "data/resource_context.h"
#include "middleware/manager.h"
#include "model/event.h"

namespace ramen_bot {
class Bot {
  SINGLETON(Bot);

public:
  bool init();

  void process(std::shared_ptr<Event>);

private:
  void process_real(std::shared_ptr<Event>);

private:
  ResourceContext& resource_context_;
  MiddlewareManager middleware_manager_;
  ThreadPool pool_;
};
}  // namespace ramen_bot