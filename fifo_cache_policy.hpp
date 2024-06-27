#ifndef FIFO_CACHE_POLICY_HPP
#define FIFO_CACHE_POLICY_HPP
#include "cache_policy.hpp"
#include <list>
#include <unordered_map>

template <typename Key>
class FIFOCachePolicy : public ICachePolicy<Key>
{
  private:
    using fifo_iterator = typename std::list<Key>::const_iterator;
    std::list<Key> fifo_queue;
    std::unordered_map<Key, fifo_iterator> key_lookup;
  public:
    
    FIFOCachePolicy() = default;
    ~FIFOCachePolicy() = default;

    void Insert(const Key &key) override
    {
        fifo_queue.emplace_front(key);
        key_lookup[key] = fifo_queue.begin();
    }
    void Move(const Key &key) override
    {
        (void)key;
    }
    void Erase(const Key &key) noexcept override
    {
        auto element = key_lookup[key];
        fifo_queue.erase(element);
        key_lookup.erase(key);
    }
    const Key &ReplCandidate() const noexcept override
    {
        return fifo_queue.back();
    }

};

#endif 
