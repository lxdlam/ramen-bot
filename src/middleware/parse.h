#pragma once

#include "marco.h"

namespace ramen_bot {
DEFINE_MIDDLEWARE_BEGIN(ParseMiddleware, "parse")

public:
bool process(std::shared_ptr<Event>) override final;
bool configure(const toml::value&) override final;

private:
std::string prefix;
bool at_necessary = false;
DEFINE_MIDDLEWARE_END(ParseMiddleware, "parse")

}  // namespace ramen_bot