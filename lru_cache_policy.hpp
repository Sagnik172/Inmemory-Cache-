#ifndef LRU_CACHE_POLICY_HPP
#define LRU_CACHE_POLICY_HPP

#include "cache_policy.hpp"
#include <list>
#include <unordered_map>

template <typename Key>
class LRUCachePolicy : public ICachePolicy<Key>
{
    private:
    using lru_iterator = typename std::list<Key>::iterator;
    std::list<Key> lru_queue;
    std::unordered_map<Key, lru_iterator> key_finder;
  public:
    

    LRUCachePolicy() = default;
    ~LRUCachePolicy() = default;

    void Insert(const Key &key) override
    {
        lru_queue.emplace_front(key);
        key_finder[key] = lru_queue.begin();
    }

    void Move(const Key &key) override
    {      
        lru_queue.splice(lru_queue.begin(), lru_queue, key_finder[key]);
    }

    void Erase(const Key &) override
    {
        key_finder.erase(lru_queue.back());
        lru_queue.pop_back();
    }

    const Key &ReplCandidate() const override
    {
        return lru_queue.back();
    }


};

#endif