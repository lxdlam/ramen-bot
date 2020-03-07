#pragma once

#include <cqcppsdk/cqcppsdk.h>

#include <variant>

namespace ramen_bot {
// 用 std::variant 包成一个单独的事件类型，用于统一过 filter
using RawEvent =
    std::variant<cq::PrivateMessageEvent, cq::GroupMessageEvent, cq::DiscussMessageEvent, cq::GroupUploadEvent,
                 cq::GroupAdminEvent, cq::GroupMemberDecreaseEvent, cq::GroupMemberIncreaseEvent, cq::GroupBanEvent,
                 cq::FriendAddEvent, cq::FriendRequestEvent, cq::GroupRequestEvent>;

template <typename T, typename V>
struct is_variant_member;

template <typename T, typename... Ts>
struct is_variant_member<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

enum class EventType {
  kMessage,
  kNotice,
  kRequest,
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

class BadEventAccessException : std::exception {};

class Event {
public:
  Event() = delete;

  constexpr explicit Event(const cq::PrivateMessageEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kMessage),
        source_type(SourceType::kPrivate),
        detail_type(DetailType::kPrivateMessage) {}

  constexpr explicit Event(const cq::GroupMessageEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kMessage),
        source_type(SourceType::kGroup),
        detail_type(DetailType::kGroupMessage) {}

  constexpr explicit Event(const cq::DiscussMessageEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kMessage),
        source_type(SourceType::kDiscuss),
        detail_type(DetailType::kDiscussMessage) {}

  constexpr explicit Event(const cq::GroupUploadEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kNotice),
        source_type(SourceType::kGroup),
        detail_type(DetailType::kGroupUpload) {}

  constexpr explicit Event(const cq::GroupAdminEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kNotice),
        source_type(SourceType::kGroup),
        detail_type(DetailType::kGroupAdmin) {}

  constexpr explicit Event(const cq::GroupMemberDecreaseEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kNotice),
        source_type(SourceType::kGroup),
        detail_type(DetailType::kGroupMemberDecrease) {}

  constexpr explicit Event(const cq::GroupMemberIncreaseEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kNotice),
        source_type(SourceType::kGroup),
        detail_type(DetailType::kGroupMemberIncrease) {}

  constexpr explicit Event(const cq::GroupBanEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kNotice),
        source_type(SourceType::kGroup),
        detail_type(DetailType::kGroupBan) {}

  constexpr explicit Event(const cq::FriendAddEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kNotice),
        source_type(SourceType::kPrivate),
        detail_type(DetailType::kFriendAdd) {}

  constexpr explicit Event(const cq::FriendRequestEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kRequest),
        source_type(SourceType::kPrivate),
        detail_type(DetailType::kFriendRequest) {}

  constexpr explicit Event(const cq::GroupRequestEvent& e) noexcept
      : raw_event(e),
        event_type(EventType::kRequest),
        source_type(SourceType::kGroup),
        detail_type(DetailType::kGroupRequest) {}

public:
  RawEvent get_raw_event() const { return raw_event; }

  template <typename E, std::enable_if_t<is_variant_member<E, RawEvent>::value, int> = 0>
  E& get_event_as() {
    return std::get<E>(raw_event);
  }

  // Helpers
public:
  constexpr bool is_message() const noexcept { return event_type == EventType::kMessage; }
  constexpr bool is_notice() const noexcept { return event_type == EventType::kNotice; }
  constexpr bool is_request() const noexcept { return event_type == EventType::kRequest; }

  constexpr bool is_private() const noexcept { return source_type == SourceType::kPrivate; }
  constexpr bool is_group() const noexcept { return source_type == SourceType::kGroup; }
  constexpr bool is_discuss() const noexcept { return source_type == SourceType::kDiscuss; }

  constexpr bool is_private_message() const noexcept { return detail_type == DetailType::kPrivateMessage; }
  constexpr bool is_group_message() const noexcept { return detail_type == DetailType::kGroupMessage; }
  constexpr bool is_discuss_message() const noexcept { return detail_type == DetailType::kDiscussMessage; }
  constexpr bool is_group_upload() const noexcept { return detail_type == DetailType::kGroupUpload; }
  constexpr bool is_group_admin() const noexcept { return detail_type == DetailType::kGroupAdmin; }
  constexpr bool is_group_member_decrease() const noexcept { return detail_type == DetailType::kGroupMemberDecrease; }
  constexpr bool is_group_member_increase() const noexcept { return detail_type == DetailType::kGroupMemberIncrease; }
  constexpr bool is_group_ban() const noexcept { return detail_type == DetailType::kGroupBan; }
  constexpr bool is_friend_add() const noexcept { return detail_type == DetailType::kFriendAdd; }
  constexpr bool is_friend_request() const noexcept { return detail_type == DetailType::kFriendRequest; }
  constexpr bool is_group_request() const noexcept { return detail_type == DetailType::kGroupRequest; }

private:
  const RawEvent raw_event;

  const EventType event_type;
  const SourceType source_type;
  const DetailType detail_type;
};
}  // namespace ramen_bot