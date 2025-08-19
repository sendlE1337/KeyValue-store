#ifndef LRU_H
#define LRU_H

#include "Policy.h"
#include <exception>
#include <list>
#include <unordered_map>

template <typename Key>
class LRUEviction : public IevictionPolicy<Key>
{
private:
  std::list<Key> lru_order;
  std::unordered_map<Key, typename std::list<Key>::iterator> lru_key_iterator;

public:
  void notifyAccess(const Key &key) override
  {
    auto it = lru_key_iterator.find(key);
    if (it != lru_key_iterator.end())
    {
      lru_order.erase(it->second);
      lru_key_iterator.erase(it);
    }

    lru_order.push_back(key);
    lru_key_iterator[key] = --lru_order.end();
  }

  std::optional<Key> requestEviction() override
  {
    if (lru_order.empty())
      return std::nullopt;

    Key key_to_rem = lru_order.front();

    lru_order.pop_front();
    lru_key_iterator.erase(key_to_rem);

    return key_to_rem;
  }

  void clear() noexcept
  {
    lru_order.clear();
    lru_key_iterator.clear();
  }

  void remove(const Key &key) override
  {
    auto it = lru_key_iterator.find(key);
    if (it != lru_key_iterator.end())
    {
      lru_order.erase(it->second); // Удаляем по итератору
      lru_key_iterator.erase(it);  // Чистим мапу
    }
  }
};

#endif