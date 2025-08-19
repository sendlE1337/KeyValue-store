#ifndef ICACHE_H
#define ICACHE_H
#include <optional>
template <typename Key, typename Value>
class ICache
{
public:
  virtual ~ICache() = default;

  virtual void put(const Key &key, const Value &value) = 0;
  virtual std::optional<Value> get(const Key &key) = 0;
  virtual void remove(const Key &key) = 0;
  virtual void clear() = 0;
  virtual size_t size() const = 0;
};

#endif