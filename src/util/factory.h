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
    if (auto itr = map_.find(key); itr != map_.end()) {
      return itr->second(std::forward<Args>(args)...);
    } else {
      return nullptr;
    }
  }

  bool register_factory(const K& key, CreativeMethodType cm) {
    if (map_.count(key) == 0) {
      map_.insert({key, std::move(cm)});
      return true;
    } else {
      return false;
    }
  }

  bool contains(const K& key) {
    auto itr = map_.find(key);
    return itr != map_.end();
  }

private:
  Factory() = default;

  std::unordered_map<KeyType, CreativeMethodType> map_;
};
}  // namespace ramen_bot