#ifndef CACHE_HPP
#define CACHE_HPP
#include "cache_policy.hpp"
#include "lru_cache_policy.hpp"
#include "fifo_cache_policy.hpp"
#include "lfu_cache_policy.hpp"
#include "cache_factory.hpp"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <pthread.h>


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
    mutable pthread_mutex_t cache_mutex;

    public:

    explicit fixed_sized_cache(size_t max_size,std::string s="LRU"):max_cache_size{max_size}
    {
        cache_policy=CacheFactory<Key>::getPolicyInstance(s);

        if (max_cache_size == 0)
        {
            throw std::invalid_argument{"Size of the cache should be non-zero"};
        }
                // Initialize the mutex
        pthread_mutex_init(&cache_mutex, nullptr);
    }

    ~fixed_sized_cache()
    {
        Clear();
        delete(cache_policy);
        // Destroy the mutex
        pthread_mutex_destroy(&cache_mutex);
        
    }

   
    void Put(const Key &key, const Value &value)
    {
        pthread_mutex_lock(&cache_mutex);
        // std::cout<<"hi";
        // cout<<key<<" "<<value<<endl;
        auto elem_it = findElement(key);

        if (elem_it == cache_items_map.end())
        {
            if (cache_items_map.size() + 1 > max_cache_size)
            {
                auto disp_candidate_key = cache_policy->ReplCandidate();

                Erase(disp_candidate_key);
            }
            std::cout << "put : (" << key << " added, value " << value << ")" << std::endl;

            Insert(key, value);
        }
        else
        {
            std::cout << "put : (" << key << " updated , value " << value << ")" << std::endl;
            Update(key, value);
        }
        pthread_mutex_unlock(&cache_mutex);
    }

    std::pair<value_type, bool> tryGet(const Key &key) const noexcept
    {
        pthread_mutex_lock(&cache_mutex); 
        const auto result = getIter(key);
        auto ans=std::make_pair(result.second ? result.first->second : nullptr,
                              result.second);
        pthread_mutex_unlock(&cache_mutex);
        return ans;
    }

    value_type Get(const Key &key) const
    {   
        pthread_mutex_lock(&cache_mutex);    
        auto elem = getIter(key);
        if (elem.second)
        {
            std::cout << "get : (" << key << ", " << *elem.first->second << ")" << std::endl;
            pthread_mutex_unlock(&cache_mutex);
            return elem.first->second;
        }
        else
        {
            pthread_mutex_unlock(&cache_mutex);
            std::cout << "get : (" << key << "  not found )" << std::endl;
            throw std::range_error{"No such element in the cache"};
        }
    }

    
    bool isCached(const Key &key) const noexcept
    {
        pthread_mutex_lock(&cache_mutex);          
        bool elem=findElement(key) != cache_items_map.cend();
        pthread_mutex_unlock(&cache_mutex);
        return elem;
    }

    std::size_t Size() const
    {
        pthread_mutex_lock(&cache_mutex);   
        size_t elem=cache_items_map.size();
        pthread_mutex_unlock(&cache_mutex);
        return elem;
    }


    bool Remove(const Key &key)
    {
        pthread_mutex_lock(&cache_mutex);   
        auto elem = findElement(key);
        if (elem == cache_items_map.end())
        {
            pthread_mutex_unlock(&cache_mutex);
            return false;
        }
        Erase(elem);
        pthread_mutex_unlock(&cache_mutex);
        return true;
    }

  protected:
    void Clear()
    {
        pthread_mutex_lock(&cache_mutex);      
        for(const std::pair<const Key, value_type> &map_entries:cache_items_map)
        cache_policy->Erase(map_entries.first);

        cache_items_map.clear();
        pthread_mutex_unlock(&cache_mutex);
    }


  protected:
    void Insert(const Key &key, const Value &value)
    {
        // pthread_mutex_lock(&cache_mutex);   
        cache_policy->Insert(key);
        cache_items_map.emplace(std::make_pair(key, std::make_shared<Value>(value)));
        // pthread_mutex_unlock(&cache_mutex);
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