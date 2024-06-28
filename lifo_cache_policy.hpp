#ifndef LIFO_CACHE_POLICY_HPP
#define LIFO_CACHE_POLICY_HPP
#include "cache_policy.hpp"
#include <list>
#include <unordered_map>

template <typename Key>
class LIFOCachePolicy : public ICachePolicy<Key>
{
  private:
    using lifo_iterator = typename std::list<Key>::const_iterator;
    std::list<Key> lifo_queue;
    std::unordered_map<Key, lifo_iterator> key_lookup;
  public:
    
    LIFOCachePolicy() = default;
    ~LIFOCachePolicy() = default;

    void Insert(const Key &key) override
    {
        lifo_queue.emplace_front(key);
        key_lookup[key] = lifo_queue.begin();
    }
    void Move(const Key &key) override
    {
        (void)key;
    }
    void Erase(const Key &key) noexcept override
    {
        auto element = key_lookup[key];
        lifo_queue.erase(element);
        key_lookup.erase(key);
    }
    const Key &ReplCandidate() const noexcept override
    {
        return lifo_queue.front();
    }

};

#endif 
