#pragma once

#include <cqcppsdk/cqcppsdk.h>
#include <fmt/format.h>

#include <crossguid/guid.hpp>
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

class Event {
public:
  explicit Event(const cq::PrivateMessageEvent& e) noexcept
      : Event(e, EventType::kMessage, SourceType::kPrivate, DetailType::kPrivateMessage) {}

  explicit Event(const cq::GroupMessageEvent& e) noexcept
      : Event(e, EventType::kMessage, SourceType::kGroup, DetailType::kGroupMessage) {}

  explicit Event(const cq::DiscussMessageEvent& e) noexcept
      : Event(e, EventType::kMessage, SourceType::kDiscuss, DetailType::kDiscussMessage) {}

  explicit Event(const cq::GroupUploadEvent& e) noexcept
      : Event(e, EventType::kNotice, SourceType::kGroup, DetailType::kGroupUpload) {}

  explicit Event(const cq::GroupAdminEvent& e) noexcept
      : Event(e, EventType::kNotice, SourceType::kGroup, DetailType::kGroupAdmin) {}

  explicit Event(const cq::GroupMemberDecreaseEvent& e) noexcept
      : Event(e, EventType::kNotice, SourceType::kGroup, DetailType::kGroupMemberDecrease) {}

  explicit Event(const cq::GroupMemberIncreaseEvent& e) noexcept
      : Event(e, EventType::kNotice, SourceType::kGroup, DetailType::kGroupMemberIncrease) {}

  explicit Event(const cq::GroupBanEvent& e) noexcept
      : Event(e, EventType::kNotice, SourceType::kGroup, DetailType::kGroupBan) {}

  explicit Event(const cq::FriendAddEvent& e) noexcept
      : Event(e, EventType::kNotice, SourceType::kPrivate, DetailType::kFriendAdd) {}

  explicit Event(const cq::FriendRequestEvent& e) noexcept
      : Event(e, EventType::kRequest, SourceType::kPrivate, DetailType::kFriendRequest) {}

  explicit Event(const cq::GroupRequestEvent& e) noexcept
      : Event(e, EventType::kRequest, SourceType::kGroup, DetailType::kGroupRequest) {}

public:
  Event(const Event&) = default;
  Event(Event&&) = default;

  Event& operator=(const Event&) = default;
  Event& operator=(Event&&) = default;

public:
  RawEvent get_raw_event() const { return raw_event_; }

  template <typename E, std::enable_if_t<is_variant_member<E, RawEvent>::value, int> = 0>
  E& get_event_as() {
    return std::get<E>(raw_event_);
  }

  std::string get_id() const { return id_; }

  void block() { blocked_ = true; }
  bool is_blocked() const noexcept { return blocked_; }

  // Helpers
public:
  constexpr bool is_message() const noexcept { return event_type_ == EventType::kMessage; }
  constexpr bool is_notice() const noexcept { return event_type_ == EventType::kNotice; }
  constexpr bool is_request() const noexcept { return event_type_ == EventType::kRequest; }

  constexpr bool is_private() const noexcept { return source_type_ == SourceType::kPrivate; }
  constexpr bool is_group() const noexcept { return source_type_ == SourceType::kGroup; }
  constexpr bool is_discuss() const noexcept { return source_type_ == SourceType::kDiscuss; }

  constexpr bool is_private_message() const noexcept { return detail_type_ == DetailType::kPrivateMessage; }
  constexpr bool is_group_message() const noexcept { return detail_type_ == DetailType::kGroupMessage; }
  constexpr bool is_discuss_message() const noexcept { return detail_type_ == DetailType::kDiscussMessage; }
  constexpr bool is_group_upload() const noexcept { return detail_type_ == DetailType::kGroupUpload; }
  constexpr bool is_group_admin() const noexcept { return detail_type_ == DetailType::kGroupAdmin; }
  constexpr bool is_group_member_decrease() const noexcept { return detail_type_ == DetailType::kGroupMemberDecrease; }
  constexpr bool is_group_member_increase() const noexcept { return detail_type_ == DetailType::kGroupMemberIncrease; }
  constexpr bool is_group_ban() const noexcept { return detail_type_ == DetailType::kGroupBan; }
  constexpr bool is_friend_add() const noexcept { return detail_type_ == DetailType::kFriendAdd; }
  constexpr bool is_friend_request() const noexcept { return detail_type_ == DetailType::kFriendRequest; }
  constexpr bool is_group_request() const noexcept { return detail_type_ == DetailType::kGroupRequest; }

private:
  // Real Default Initilizer
  Event(RawEvent&& e, EventType event_type, SourceType source_type, DetailType detail_type)
      : raw_event_(e),
        event_type_(event_type),
        source_type_(source_type),
        detail_type_(detail_type),
        id_(std::move(xg::newGuid().str())),
        blocked_(false) {}

private:
  RawEvent raw_event_;

  const EventType event_type_;
  const SourceType source_type_;
  const DetailType detail_type_;

  const std::string id_;

  bool blocked_;
};
}  // namespace ramen_bot

namespace fmt {
template <>
struct formatter<ramen_bot::Event> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const ramen_bot::Event& e, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", e.get_id());
  }
};

template <>
struct formatter<std::shared_ptr<ramen_bot::Event>> : formatter<ramen_bot::Event> {
  template <typename FormatContext>
  auto format(const std::shared_ptr<ramen_bot::Event>& e, FormatContext& ctx) {
    return formatter<ramen_bot::Event>::format(*e, ctx);
  }
};
}  // namespace fmt