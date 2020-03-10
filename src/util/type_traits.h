#pragma once

namespace ramen_bot {
template <typename T, typename V>
struct is_variant_member;

template <typename T, typename... Ts>
struct is_variant_member<T, std::variant<Ts...>> : std::disjunction<std::is_same<T, Ts>...> {};

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overload(Ts...)->overload<Ts...>;
}  // namespace ramen_bot