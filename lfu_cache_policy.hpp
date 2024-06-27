#ifndef LFU_CACHE_POLICY_HPP
#define LFU_CACHE_POLICY_HPP
#include "cache_policy.hpp"
#include <cstddef>
#include <iostream>
#include <map>
#include <unordered_map>
template <typename Key>
class LFUCachePolicy : public ICachePolicy<Key>
{
  private:
  using lfu_iterator = typename std::multimap<std::size_t, Key>::iterator;
    std::multimap<std::size_t, Key> frequency_storage;
    std::unordered_map<Key, lfu_iterator> lfu_storage;
  public:
    

    LFUCachePolicy() = default;
    ~LFUCachePolicy() override = default;

    void Insert(const Key &key) override
    {
        std::size_t INIT_VAL = 1;
        lfu_storage[key] =
        frequency_storage.emplace_hint(frequency_storage.cbegin(), INIT_VAL, key);
    }

    void Move(const Key &key) override
    {
        auto elem_for_update = lfu_storage[key];
        auto updated_elem = std::make_pair(elem_for_update->first + 1, elem_for_update->second);
        frequency_storage.erase(elem_for_update);
        lfu_storage[key] =
        frequency_storage.emplace_hint(frequency_storage.cend(), std::move(updated_elem));
    }

    void Erase(const Key &key) override
    {
        frequency_storage.erase(lfu_storage[key]);
        lfu_storage.erase(key);
    }

    const Key &ReplCandidate() const noexcept override
    {
        return frequency_storage.cbegin()->second;
    }

 
};
#endif 
