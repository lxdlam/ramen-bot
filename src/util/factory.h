#pragma once

#include "common/common_def.h"

namespace ramen_bot {
/*
 * 简单的抽象工厂，由于使用 static 变量如果定义到不同的翻译单元可能出现其他问题，所以使用一个简单的单例来替代。
 * 模板参数分别为 Key 的类型，对象的基类类型以及创建对象需要的参数类型
 */
template <typename K, typename E, typename... Args>
class Factory {
public:
  using KeyType = K;
  using EntityType = E;
  using CreativeMethodType = std::function<std::unique_ptr<E>(std::decay_t<Args>...)>;

  static Factory& get_instance() {
    static Factory instance_;
    return instance_;
  }

  Factory(const Factory&) = delete;
  Factory(Factory&&) = delete;
  Factory& operator=(const Factory&) = delete;
  Factory& operator=(Factory&&) = delete;

  std::unique_ptr<E> make_entity(const K& key, Args&&... args) {
    auto cm = cm_map_.find(key);
    if (!cm) {
      return nullptr;
    } else {
      return cm->operator()(std::forward<Args>(args)...);
    }
  }

  bool register_entity(const K& key, CreativeMethodType cm) { return cm_map_.add(key, std::move(cm)); }

private:
  Factory() = default;

  class {
  public:
    std::optional<CreativeMethodType> find(const K& key) {
      auto& map = get_map_instance();
      if (auto itr = map.find(key); itr != map.end()) {
        return itr->second;
      } else {
        return std::nullopt;
      }
    }

    bool add(const K& key, CreativeMethodType cm) {
      auto& map = get_map_instance();
      if (map.count(key) == 0) {
        map.insert({key, std::move(cm)});
        return true;
      } else {
        return false;
      }
    }

  private:
    static std::unordered_map<KeyType, CreativeMethodType>& get_map_instance() {
      static std::unordered_map<KeyType, CreativeMethodType> instance_;
      return instance_;
    }
  } cm_map_;
};
}  // namespace ramen_bot