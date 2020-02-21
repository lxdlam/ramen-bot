#pragma once

#include <cqcppsdk/cqcppsdk.h>

#include <variant>

namespace ramen_bot {
// 以下事件均被重新定义过
using RawEvent =
    std::variant<cq::PrivateMessageEvent, cq::GroupMessageEvent, cq::DiscussMessageEvent, cq::GroupUploadEvent,
                 cq::GroupAdminEvent, cq::GroupMemberDecreaseEvent, cq::GroupMemberIncreaseEvent, cq::GroupBanEvent,
                 cq::FriendAddEvent, cq::FriendRequestEvent, cq::GroupRequestEvent>;

enum class EventType {
  kMessageType,
  kNoticeType,
  kRequestType,
};

enum class SourceType {
  kPrivate,
  kGroup,
  kDiscuss,
};

enum class DetailType {
  kPrivateMessage,
  kGroupMessage,
  kDiscussMessage,
  kGroupUpload,
  kGroupAdmin,
  kGroupMemberDecrease,
  kGroupMemberIncrease,
  kGroupBan,
  kFriendAdd,
  kFriendRequest,
  kGroupRequest,
};

struct Event {};

}  // namespace ramen_bot