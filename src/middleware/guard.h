#pragma once

#include "marco.h"

namespace ramen_bot {
DEFINE_MIDDLEWARE_BEGIN(GuardMiddleware, "guard")

public:
bool process(std::shared_ptr<Event>) override final;
bool configure(const toml::value&) override final;

private:
void drop_message(std::shared_ptr<Event> e);
void accept_message(std::shared_ptr<Event> e);

private:
std::unordered_set<std::string> accept_events_;
bool accept_all_events_ = false;
bool drop_self_ = false;

DEFINE_MIDDLEWARE_END(GuardMiddleware, "guard")

void block_raw(std::shared_ptr<Event>);

}  // namespace ramen_bot