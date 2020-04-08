#pragma once

#include <cqcppsdk/cqcppsdk.h>
#include <fmt/format.h>

#include <crossguid/guid.hpp>
#include <variant>

#include "message.h"
#include "util/type_traits.h"

namespace ramen_bot {
// 用 std::variant 包成一个单独的事件类型，用于统一过 filter
using RawEvent =
    std::variant<cq::PrivateMessageEvent, cq::GroupMessageEvent, cq::DiscussMessageEvent, cq::GroupUploadEvent,
                 cq::GroupAdminEvent, cq::GroupMemberDecreaseEvent, cq::GroupMemberIncreaseEvent, cq::GroupBanEvent,
                 cq::FriendAddEvent, cq::FriendRequestEvent, cq::GroupRequestEvent>;

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
  RawEvent& get_raw_event() { return raw_event_; }

  template <typename E, std::enable_if_t<is_variant_member<E, RawEvent>::value, int> = 0>
  E& get_event_as() {
    return std::get<E>(raw_event_);
  }

  template <typename V>
  constexpr auto visit_raw(V&& v) {
    return std::visit(std::forward<V>(v), raw_event_);
  }

  std::string_view get_id() const { return id_; }

  void block() { blocked_ = true; }
  bool is_blocked() const noexcept { return blocked_; }
  void block_raw() {
    std::visit(overload{
                   [](cq::PrivateMessageEvent& e) { e.block(); },
                   [](cq::GroupMessageEvent& e) { e.block(); },
                   [](cq::DiscussMessageEvent& e) { e.block(); },
                   [](cq::GroupUploadEvent& e) { e.block(); },
                   [](cq::GroupAdminEvent& e) { e.block(); },
                   [](cq::GroupMemberDecreaseEvent& e) { e.block(); },
                   [](cq::GroupMemberIncreaseEvent& e) { e.block(); },
                   [](cq::GroupBanEvent& e) { e.block(); },
                   [](cq::FriendAddEvent& e) { e.block(); },
                   [](cq::FriendRequestEvent& e) { e.block(); },
                   [](cq::GroupRequestEvent& e) { e.block(); },
               },
               raw_event_);
  }

  constexpr EventType get_event_type() const noexcept { return event_type_; }
  constexpr SourceType get_source_type() const noexcept { return source_type_; }
  constexpr DetailType get_detail_type() const noexcept { return detail_type_; }

private:
  // Real Default Initilizer
  Event(RawEvent&& e, EventType event_type, SourceType source_type, DetailType detail_type)
      : raw_event_(e),
        event_type_(event_type),
        source_type_(source_type),
        detail_type_(detail_type),
        id_(std::move(xg::newGuid().str())),
        blocked_(false) {}

public:
  // Message message_;

private:
  RawEvent raw_event_;

  EventType event_type_;
  SourceType source_type_;
  DetailType detail_type_;

  std::string id_;

  bool blocked_;
};
}  // namespace ramen_bot

namespace fmt {
template <>
struct formatter<ramen_bot::Event> {
  bool full = false;
  inline static const char* const full_layout =
      "Event{{id={}, event_type={}, source_type={}, detail_type={}, blocked={}}}";

  constexpr auto parse(format_parse_context& ctx) {
    auto it = ctx.begin(), end = ctx.end();
    if (it != end && *it == 'f') {
      full = true;
      it++;
    }

    if (it != end && *it != '}') {
      throw format_error("invalid format");
    }

    return it;
  }

  template <typename FormatContext>
  auto format(const ramen_bot::Event& e, FormatContext& ctx) {
    if (full) {
      return format_to(ctx.out(), full_layout, e.get_id(), e.get_event_type(), e.get_source_type(), e.get_detail_type(),
                       e.is_blocked());
    } else {
      return format_to(ctx.out(), "{}", e.get_id());
    }
  }
};

template <>
struct formatter<std::shared_ptr<ramen_bot::Event>> : formatter<ramen_bot::Event> {
  template <typename FormatContext>
  auto format(const std::shared_ptr<ramen_bot::Event>& e, FormatContext& ctx) {
    return formatter<ramen_bot::Event>::format(*e, ctx);
  }
};

template <>
struct formatter<ramen_bot::EventType> : formatter<std::string_view> {
  template <typename FormatContext>
  auto format(ramen_bot::EventType event_type, FormatContext& ctx) {
    std::string_view name = "unknown";
    switch (event_type) {
      case ramen_bot::EventType::kMessage:
        name = "message";
        break;
      case ramen_bot::EventType::kNotice:
        name = "notice";
        break;
      case ramen_bot::EventType::kRequest:
        name = "request";
        break;
    }
    return formatter<std::string_view>::format(name, ctx);
  }
};

template <>
struct formatter<ramen_bot::SourceType> : formatter<std::string_view> {
  template <typename FormatContext>
  auto format(ramen_bot::SourceType source_type, FormatContext& ctx) {
    std::string_view name = "unknown";
    switch (source_type) {
      case ramen_bot::SourceType::kPrivate:
        name = "private";
        break;
      case ramen_bot::SourceType::kGroup:
        name = "group";
        break;
      case ramen_bot::SourceType::kDiscuss:
        name = "discuss";
        break;
    }
    return formatter<std::string_view>::format(name, ctx);
  }
};

template <>
struct formatter<ramen_bot::DetailType> : formatter<std::string_view> {
  template <typename FormatContext>
  auto format(ramen_bot::DetailType detail_type, FormatContext& ctx) {
    std::string_view name = "unknown";
    switch (detail_type) {
      case ramen_bot::DetailType::kPrivateMessage:
        name = "private_message";
        break;
      case ramen_bot::DetailType::kGroupMessage:
        name = "group_message";
        break;
      case ramen_bot::DetailType::kDiscussMessage:
        name = "discuss_message";
        break;
      case ramen_bot::DetailType::kGroupUpload:
        name = "group_upload";
        break;
      case ramen_bot::DetailType::kGroupAdmin:
        name = "group_admin";
        break;
      case ramen_bot::DetailType::kGroupMemberDecrease:
        name = "group_member_decrease";
        break;
      case ramen_bot::DetailType::kGroupMemberIncrease:
        name = "group_member_increase";
        break;
      case ramen_bot::DetailType::kGroupBan:
        name = "group_ban";
        break;
      case ramen_bot::DetailType::kFriendAdd:
        name = "friend_add";
        break;
      case ramen_bot::DetailType::kFriendRequest:
        name = "friend_request";
        break;
      case ramen_bot::DetailType::kGroupRequest:
        name = "group_request";
        break;
    }
    return formatter<std::string_view>::format(name, ctx);
  }
};
}  // namespace fmt