#pragma once

namespace ramen_bot {
enum class Authority {
  kNormalUser = 1,         // 所有用户
  kGroupMember = 1 << 1,   // 群成员
  kGroupManager = 1 << 2,  // 管理员
  kGroupOwner = 1 << 3,    // 群主
  kSuperUser = 1 << 4,     // 超级管理员
};

enum class MessageSource { kPrivate, kGroup, kOldDiscuss };

enum class GroupEventType {
  kFileUpload,
  kAdminChange,
  kMemberAdd,
  kMemberLeave,
  kBan,
  kFriendAdd
};
}  // namespace ramen_bot