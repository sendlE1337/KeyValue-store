#ifndef LRUCACHE_H
#define LRUCACHE_H

#include "ICache.h"
#include "eviction/Policy.h"
#include <unordered_map>
#include <memory>

template <typename Key, typename Value>
class GenericCache : public ICache<Key, Value>
{
private:
  size_t m_capacity;
  std::unique_ptr<IevictionPolicy<Key>> m_eviction;
  std::unordered_map<Key, Value> m_cache;

public:
  GenericCache(size_t capacity, std::unique_ptr<IevictionPolicy<Key>> evictionPolicy) : m_capacity(capacity), m_eviction(std::move(evictionPolicy))
  {
  }

  void put(const Key &key, const Value &value) override
  {
    if (auto it = m_cache.find(key); it != m_cache.end())
    {
      it->second = std::move(value);
      m_eviction->notifyAccess(key);
      return;
    }

    if (m_cache.size() >= m_capacity)
    {
      if (auto key_to_remove = m_eviction->requestEviction())
        m_cache.erase(*key_to_remove);
    }

    m_cache.emplace(key, std::move(value));
    m_eviction->notifyAccess(key);
  }

  std::optional<Value> get(const Key &key) override
  {
    auto it = m_cache.find(key);
    if (it == m_cache.end())
      return std::nullopt;

    m_eviction->notifyAccess(key);
    return it->second;
  }

  void clear() override
  {
    m_cache.clear();
  }

  void remove(const Key &key) override
  {
    m_cache.erase(key);
    m_eviction->remove(key);
  }

  size_t size() const override
  {
    return m_cache.size();
  }
};

#endif