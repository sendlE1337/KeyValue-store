#ifndef FACADE_H
#define FACADE_H
#include "generic_cache.h"
#include "config.h"
#include "cache/eviction/lru.h"

template <typename Key, typename Value>
class CacheFacade
{
public:
  CacheFacade(size_t capacity, const std::string &policy = "LRU")
  {
    if (capacity <= 0)
      throw std::invalid_argument("Cache capacity cannot be zero");

    constexpr size_t MAX_CACHE_SIZE = 10000;
    if (capacity > MAX_CACHE_SIZE)
      capacity = MAX_CACHE_SIZE;

    initialize_cache(capacity, policy);
  }
  explicit CacheFacade(const std::string &config) : config_(config)
  {
    try
    {
      if (!config_.is_valid())
        config_.reset_to_default();

      initialize_cache(config_.getCacheLimit(), config_.getEvictionPolicy());
    }
    catch (...)
    {
      config_.reset_to_default();
      initialize_cache(config_.getCacheLimit(), config_.getEvictionPolicy());
    }
  }

  void put(const Key &key, const Value &value)
  {
    cache_->put(key, std::move(value));
  }
  std::optional<Value> get(const Key &key)
  {
    return cache_->get(key);
  }
  void clear()
  {
    cache_->clear();
  }
  void remove(const Key &key)
  {
    cache_->remove(key);
  }
  size_t size() const
  {
    return cache_->size();
  }

private:
  std::unique_ptr<GenericCache<Key, Value>> cache_;
  Config config_;

  void initialize_cache(size_t capacity, const std::string &policy)
  {
    if (policy == "LRU")
    {
      cache_ = std::make_unique<GenericCache<Key, Value>>(capacity, std::make_unique<LRUEviction<Key>>());
    }
    else
    {
      throw std::invalid_argument("Unsupported eviction policy: " + policy);
    }
  }
};

#endif