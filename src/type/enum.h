#pragma once

namespace ramen_bot {
namespace type {
enum class Authority {
  kNormalUser = 1,         // 所有用户
  kGroupMember = 1 << 1,   // 群成员
  kGroupManager = 1 << 2,  // 管理员
  kGroupOwner = 1 << 3,    // 群主
  kSuperUser = 1 << 4,     // 超级管理员
};
}
}  // namespace ramen_bot