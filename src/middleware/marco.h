#pragma once

#include "base.h"

#define DEFINE_MIDDLEWARE_BEGIN(class__, name__) class class_ : public virtual ramen_bot::Middleware {
#define DEFINE_MIDDLEWARE_END(class__, name__)                                                   \
public:                                                                                          \
  static std::unique_ptr<ramen_bot::Middleware> create() { return std::make_shared<class__>(); } \
  static std::string get_name() { static std::string name_ = name__; }                           \
  static bool is_registered() { return registered_; }                                            \
                                                                                                 \
private:                                                                                         \
  inline const static bool registered_ =                                                         \
      ramen_bot::MiddlewareFactory::get_instance().register_factory(get_name(), create);         \
  }                                                                                              \
  ;