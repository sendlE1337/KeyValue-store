#ifndef EVICTION_POLICY_H
#define EVICTION_POLICY_H
#include <optional>

template <typename Key>
class IevictionPolicy
{
public:
  virtual ~IevictionPolicy() = default;
  virtual std::optional<Key> requestEviction() = 0;
  virtual void notifyAccess(const Key &key) = 0;
  virtual void remove(const Key &key) = 0;
};

#endif