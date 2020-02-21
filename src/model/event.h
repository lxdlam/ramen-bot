#pragma once

#include <cqcppsdk/cqcppsdk.h>

#include <variant>

namespace ramen_bot {
// 以下事件均被重新定义过
using MessageEvent =
    std::variant<cq::PrivateMessageEvent, cq::GroupMessageEvent,
                 cq::DiscussMessageEvent>;
using NoticeEvent =
    std::variant<cq::GroupUploadEvent, cq::GroupAdminEvent,
                 cq::GroupMemberDecreaseEvent, cq::GroupMemberIncreaseEvent,
                 cq::GroupBanEvent, cq::FriendAddEvent>;
using RequestEvent =
    std::variant<cq::FriendRequestEvent, cq::GroupRequestEvent>;
using EventBase = std::variant<MessageEvent, NoticeEvent, RequestEvent>;

// Type traits
template <typename T, typename V>
struct isVariantMember;

template <typename T, typename... Ts>
struct isVariantMember<T, std::variant<Ts...>>
    : std::disjunction<std::is_same<T, Ts>...> {};

template <typename E, typename V>
std::enable_if_t<isVariantMember<E, V>::value, E> get_member(V variant) {
  static_assert(std::holds_alternative<E, V>,
                "the variant is not held the given type");

  return std::get<E>(variant);
}

template <typename E>
struct isMessageEvent : isVariantMember<E, MessageEvent> {};

template <typename E>
struct isNoticeEvent : isVariantMember<E, NoticeEvent> {};

template <typename E>
struct isRequestEvent : isVariantMember<E, RequestEvent> {};

struct Event {
  EventBase base_event;

  template <typename E>
  E get_detail_event() {
    if constexpr (isMessageEvent<E>::value) {
      return get_member<E, MessageEvent>(get_sub_event<MessageEvent>());
    } else {
      if constexpr (isNoticeEvent<E>::value) {
        return get_member<E, NoticeEvent>(get_sub_event<NoticeEvent>());
      } else {
        if constexpr (isRequestEvent<E>::value) {
          return get_member<E, RequestEvent>(get_sub_event<RequestEvent>());
        } else {
          throw std::bad_variant_access("unknown type is given");
        }
      }
    }
  }

  EventBase get_base_event() { return base_event; }

  template <typename SE>
  SE get_sub_event() {
    return get_member<SE, EventBase>(base_event);
  }
};
}  // namespace ramen_bot