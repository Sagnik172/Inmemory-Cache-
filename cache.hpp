#ifndef CACHE_HPP
#define CACHE_HPP
#include "cache_policy.hpp"
#include "lru_cache_policy.hpp"
#include "cache_factory.hpp"
#include <algorithm>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <unordered_map>


template <typename V>
using WrappedValue = std::shared_ptr<V>;
template <typename Key, typename Value>
class fixed_sized_cache
{
    
  private:
    using map_type =typename std::unordered_map<Key, WrappedValue<Value>>;
    using value_type = typename map_type::mapped_type;
    using const_iter = typename map_type::const_iterator;
    map_type cache_items_map;
    mutable ICachePolicy<Key> *cache_policy;
    std::size_t max_cache_size;

    public:

    explicit fixed_sized_cache(size_t max_size,std::string s="LRUCache"):max_cache_size{max_size}
    {
        cache_policy=CacheFactory<Key>::getPolicyInstance(s);

        if (max_cache_size == 0)
        {
            throw std::invalid_argument{"Size of the cache should be non-zero"};
        }
    }

    ~fixed_sized_cache()
    {
        Clear();
        delete(cache_policy);
    }

   
    void Put(const Key &key, const Value &value)
    {
        auto elem_it = findElement(key);

        if (elem_it == cache_items_map.end())
        {
            if (cache_items_map.size() + 1 > max_cache_size)
            {
                auto disp_candidate_key = cache_policy->ReplCandidate();

                Erase(disp_candidate_key);
            }

            Insert(key, value);
        }
        else
        {
            Update(key, value);
        }
    }

    std::pair<value_type, bool> tryGet(const Key &key) const noexcept
    {
        const auto result = getIter(key);
        return std::make_pair(result.second ? result.first->second : nullptr,
                              result.second);
    }

    value_type Get(const Key &key) const
    {       
        auto elem = getIter(key);
        if (elem.second)
        {
            return elem.first->second;
        }
        else
        {
            throw std::range_error{"No such element in the cache"};
        }
    }

    
    bool isCached(const Key &key) const noexcept
    {       
        return findElement(key) != cache_items_map.cend();
    }

    std::size_t Size() const
    {
        return cache_items_map.size();
    }


    bool Remove(const Key &key)
    {
        auto elem = findElement(key);
        if (elem == cache_items_map.end())
        {
            return false;
        }
        Erase(elem);
        return true;
    }

  protected:
    void Clear()
    {
        for(const std::pair<const Key, value_type> &map_entries:cache_items_map)
        cache_policy->Erase(map_entries.first);
        cache_items_map.clear();
    }


  protected:
    void Insert(const Key &key, const Value &value)
    {
        cache_policy->Insert(key);
        cache_items_map.emplace(std::make_pair(key, std::make_shared<Value>(value)));
    }

    void Erase(const_iter elem)
    {
        cache_policy->Erase(elem->first);
        cache_items_map.erase(elem);
    }

    void Erase(const Key &key)
    {
        auto elem_it = findElement(key);
        Erase(elem_it);
    }

    void Update(const Key &key, const Value &value)
    {
        cache_policy->Move(key);
        cache_items_map[key] = std::make_shared<Value>(value);
    }

    const_iter findElement(const Key &key) const
    {
        return cache_items_map.find(key);
    }

    std::pair<const_iter, bool> getIter(const Key &key) const noexcept
    {
        auto elem_it = findElement(key);

        if (elem_it != cache_items_map.cend())
        {
            cache_policy->Move(key);
            return {elem_it, true};
        }

        return {elem_it, false};
    }

};
#endif 