#pragma once

#define NO_COPY_MOVE(_name)                \
  _name(const _name&) = delete;            \
  _name& operator=(const _name&) = delete; \
  _name(_name&&) = delete;                 \
  _name& operator=(_name&&) = delete;

#define SINGLETON(_name)         \
public:                          \
  static _name& get_instance() { \
    static _name _instance;      \
    return _instance;            \
  }                              \
  NO_COPY_MOVE(_name)            \
private:                         \
  _name();
